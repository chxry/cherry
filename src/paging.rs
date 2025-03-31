use core::arch::asm;
use fdt::Fdt;
use crate::kprintln;
use crate::symbols::{_kernel_start, _heap_start};

const PAGE_SIZE: usize = 4096;
const SV39_MODE: usize = 8;

pub fn init(fdt: &Fdt) {
  for r in fdt.memory().regions() {
    // todo consider this in alloc_page
    kprintln!("memory avail = {} B", r.size.unwrap());
  }
  unsafe {
    let root_addr = tmp_alloc_page();
    let root = &mut *(root_addr as *mut PageTable);
    root.identity_map_range(
      &raw const _kernel_start as _,
      &raw const _heap_start as _,
      EntryFlags::READ | EntryFlags::WRITE | EntryFlags::EXECUTE,
    );
    // todo the driver should map itself
    root.identity_map_range(0x10000000, 0x10000100, EntryFlags::READ | EntryFlags::WRITE);
    // root.print(2, 0); // must print before since the heap isnt identity mapped

    let satp_val = (SV39_MODE << 60) | root_addr as usize >> 12;
    asm!(
      "csrw satp, {}",
      "sfence.vma",
      in(reg) satp_val
    );
  }
}

/// aligned to PAGE_SIZE
fn tmp_alloc_page() -> *mut u8 {
  unsafe {
    static mut NEXT: *mut u8 = &raw mut _heap_start;
    let next = NEXT;
    for i in 0..PAGE_SIZE {
      *next.add(i) = 0;
    }
    NEXT = NEXT.add(PAGE_SIZE);
    next
  }
}

bitflags::bitflags! {
  #[derive(Copy,Clone)]
  struct EntryFlags: u8 {
    const VALID = 1 << 0;
    const READ = 1 << 1;
    const WRITE = 1 << 2;
    const EXECUTE = 1 << 3;
    const USER = 1 << 4;
    const GLOBAL = 1 << 5;
    const ACCESSED = 1 << 6;
    const DIRTY = 1 << 7;
  }
}

struct Entry(usize);

impl Entry {
  fn new(ppn: usize, flags: EntryFlags) -> Self {
    Self(ppn << 10 | flags.bits() as usize)
  }

  fn is_valid(&self) -> bool {
    self.0 & EntryFlags::VALID.bits() as usize != 0
  }

  fn is_leaf(&self) -> bool {
    self.0 & (EntryFlags::READ | EntryFlags::WRITE | EntryFlags::EXECUTE).bits() as usize != 0
  }
}

struct PageTable([Entry; 512]);

impl PageTable {
  fn map(&mut self, virt_addr: usize, phy_addr: usize, flags: EntryFlags) {
    let vpn = [
      (virt_addr >> 12) & 0x1ff,
      (virt_addr >> 21) & 0x1ff,
      (virt_addr >> 30) & 0x1ff,
    ];

    let mut v = &mut self.0[vpn[2]];
    for i in [1, 0] {
      if !v.is_valid() {
        let page = tmp_alloc_page();
        *v = Entry::new(page as usize >> 12, EntryFlags::VALID);
      }
      let entry = ((v.0 & !0x3ff) << 2) as *mut Entry;
      v = unsafe { entry.add(vpn[i]).as_mut().unwrap() };
    }

    *v = Entry::new(phy_addr >> 12, flags | EntryFlags::VALID);
  }

  fn identity_map_range(&mut self, start: usize, end: usize, flags: EntryFlags) {
    let start = align_down(start);
    let n = (align_up(end) - start) / PAGE_SIZE;
    for i in 0..n {
      let addr = start + PAGE_SIZE * i;
      self.map(addr, addr, flags);
    }
  }

  fn print(&self, level: usize, virt_prefix: usize) {
    for i in 0..512 {
      let entry = &self.0[i];
      if !entry.is_valid() {
        continue;
      }

      let virt_addr = virt_prefix | (i << (12 + 9 * level));
      let phy_addr = (entry.0 >> 10) << 12;
      // let flags = EntryFlags::from_bits_truncate(entry.0 as _);
      if entry.is_leaf() {
        kprintln!("{:#0x} -> {:#x}", virt_addr, phy_addr);
      } else {
        let next = unsafe { &*(phy_addr as *const Self) };
        next.print(level - 1, virt_addr);
      }
    }
  }
}

fn align_down(addr: usize) -> usize {
  addr & !(PAGE_SIZE - 1)
}

fn align_up(addr: usize) -> usize {
  (addr + PAGE_SIZE - 1) & !(PAGE_SIZE - 1)
}
