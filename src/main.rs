#![no_std]
#![no_main]
#![feature(riscv_ext_intrinsics)]
mod paging;

use core::fmt;
use core::arch::{global_asm, riscv64};
use core::panic::PanicInfo;
use fdt::Fdt;

global_asm!(include_str!("boot.s"));

pub mod symbols {
  unsafe extern "C" {
    pub static mut _kernel_start: u8;
    pub static mut _heap_start: u8;
  }
}

#[unsafe(no_mangle)]
extern "C" fn kmain(hart_id: usize, fdt_addr: usize) -> ! {
  let fdt = unsafe { Fdt::from_ptr(fdt_addr as _).unwrap() };

  kprintln!("cherry v{}", env!("CARGO_PKG_VERSION"));
  kprintln!("hart_id = {}", hart_id);
  kprintln!("model = {}", fdt.root().model());
  paging::init(&fdt);
  // !! our fdt isnt identity mapped so its invalid after this point

  loop {
    if Uart.data_avail() {
      Uart.put(Uart.read());
    }
  }
}

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
	let _ = write!(crate::Uart, $($args)+);
  });
}

#[macro_export]
macro_rules! kprintln {
  () => ({
    crate::kprint!("\r\n")
  });
  ($fmt:expr) => ({
    crate::kprint!(concat!($fmt, "\r\n"))
  });
  ($fmt:expr, $($args:tt)+) => ({
    crate::kprint!(concat!($fmt, "\r\n"), $($args)+)
  });
}

#[panic_handler]
fn panic(info: &PanicInfo) -> ! {
  // fallback to sbi here
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
