#![no_std]
#![no_main]
use core::fmt;
use core::arch::{asm, global_asm};
use core::panic::PanicInfo;

global_asm!(include_str!("boot.s"));

const UART_BASE: *mut u8 = 0x10000000 as _;

#[unsafe(no_mangle)]
extern "C" fn kmain() -> ! {
  kprintln!("cherry {}", 2);
  loop {
    if Uart.data_avail() {
      Uart.put(Uart.read());
    }
  }
}

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
  kprintln!("KERNEL PANIC!\n{}", info);
  abort()
}

fn abort() -> ! {
  loop {
    unsafe {
      asm!("wfi");
    }
  }
}
