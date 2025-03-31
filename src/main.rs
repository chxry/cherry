#![no_std]
#![no_main]
#![feature(riscv_ext_intrinsics)]
use core::fmt;
use core::arch::{global_asm, asm, riscv64};
use core::panic::PanicInfo;
use fdt::Fdt;

global_asm!(include_str!("boot.s"));

unsafe extern "C" {
  static mut _kernel_start: u8;
  static mut _heap_start: u8;
}

#[unsafe(no_mangle)]
extern "C" fn kmain(_: usize, fdt_addr: usize) -> ! {
  let fdt = unsafe { Fdt::from_ptr(fdt_addr as _).unwrap() };

  kprintln!("cherry {}", 2);
  kprintln!("model = {}", fdt.root().model());
  for m in fdt.memory().regions() {
    kprintln!("{:p} {:?}", m.starting_address, m.size);
  }

  unsafe {
    let root_addr = tmp_alloc_page();
    let root = &mut *(root_addr as *mut PageTable);
    root.identity_map_range(
      &raw const _kernel_start as _,
      &raw const _heap_start as _,
      EntryFlags::Read | EntryFlags::Write | EntryFlags::Execute,
    );
    // todo the driver should map itself
    root.identity_map_range(0x10000000, 0x10000100, EntryFlags::Read | EntryFlags::Write);

    let root_ppn = root_addr as usize >> 12;
    let satp_val = (8 << 60) | root_ppn;
    asm!(
      "csrw satp, {}",
      "sfence.vma",
      in(reg) satp_val
    );
    kprintln!("holy paged");
    asm!("csrrw x0, cycle, x0");
  }

  loop {
    if Uart.data_avail() {
      Uart.put(Uart.read());
    }
  }
}

const PAGE_SIZE: usize = 4096;

fn tmp_alloc_page() -> *mut u8 {
  unsafe {
    static mut NEXT: *mut u8 = &raw mut _heap_start;
    let next = NEXT;
    for i in 0..PAGE_SIZE {
      *next.add(i) = 0;
    }
    NEXT = NEXT.add(4096);
    next
  }
}

bitflags::bitflags! {
  #[derive(Copy,Clone)]
  struct EntryFlags: u8 {
    const Valid = 1 << 0;
    const Read = 1 << 1;
    const Write = 1 << 2;
    const Execute = 1 << 3;
    const User = 1 << 4;
  }
}

struct Entry(usize);

impl Entry {
  fn is_valid(&self) -> bool {
    self.0 & EntryFlags::Valid.bits() as usize != 0
  }
}

struct PageTable([Entry; 512]);

impl PageTable {
  fn map(&mut self, virt_addr: usize, phy_addr: usize, flags: EntryFlags) {
    let vpn = [(virt_addr >> 12) & 0x1ff, (virt_addr >> 21) & 0x1ff, (virt_addr >> 30) & 0x1ff];
    let ppn = phy_addr >> 12 & 0xfffffffffff;

    let mut v = &mut self.0[vpn[2]];
    for i in [1, 0] {
      if !v.is_valid() {
        let page = tmp_alloc_page();
        *v = Entry((page as usize >> 2) | EntryFlags::Valid.bits() as usize);
      }
      let entry = ((v.0 & !0x3ff) << 2) as *mut Entry;
      v = unsafe { entry.add(vpn[i]).as_mut().unwrap() };
    }

    *v = Entry(ppn << 10 | flags.bits() as usize | EntryFlags::Valid.bits() as usize);
  }

  pub fn identity_map_range(&mut self, start: usize, end: usize, flags: EntryFlags) {
    let start = align_down(start);
    let n = (align_up(end) - start) / PAGE_SIZE;
    for i in 0..n {
      let addr = start + PAGE_SIZE * i;
      self.map(addr, addr, flags);
    }
  }
}

pub fn align_down(addr: usize) -> usize {
  addr & !(PAGE_SIZE - 1)
}

pub fn align_up(addr: usize) -> usize {
  (addr + PAGE_SIZE - 1) & !(PAGE_SIZE - 1)
}

// trait Driver {
//   fn compat(s: &str) -> bool;
// }

const UART_BASE: *mut u8 = 0x10000000 as _;

struct Uart;

impl Uart {
  fn put(&self, x: u8) {
    unsafe { UART_BASE.write_volatile(x) }
  }

  fn read(&self) -> u8 {
    unsafe { UART_BASE.read_volatile() }
  }

  fn data_avail(&self) -> bool {
    unsafe { UART_BASE.add(5).read_volatile() & 1 != 0 }
  }
}

impl fmt::Write for Uart {
  fn write_str(&mut self, s: &str) -> fmt::Result {
    for c in s.bytes() {
      self.put(c);
    }
    Ok(())
  }
}

#[macro_export]
macro_rules! kprint {
  ($($args:tt)+) => ({
	use core::fmt::Write;
	// todo write to a ring buffer
	let _ = write!(Uart, $($args)+);
  });
}

#[macro_export]
macro_rules! kprintln {
  () => ({
    kprint!("\r\n")
  });
  ($fmt:expr) => ({
    kprint!(concat!($fmt, "\r\n"))
  });
  ($fmt:expr, $($args:tt)+) => ({
    kprint!(concat!($fmt, "\r\n"), $($args)+)
  });
}

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
  kprintln!("kernel panic!\n{}", info);
  abort()
}

#[unsafe(no_mangle)]
extern "C" fn trap_handler() -> ! {
  panic!("waaa");
}

fn abort() -> ! {
  loop {
    unsafe {
      riscv64::wfi();
    }
  }
}
