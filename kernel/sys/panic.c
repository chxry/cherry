#include "panic.h"

void panic(char* msg) {
  serial_putstr("\033[1;31mKERNEL PANIC: \033[39m");
  serial_putstr(msg);
  for (;;) {
    asm("hlt");
  }
}

void exc0() {
  panic("Division by Zero");
};

void exc1() {
  panic("Debug");
};

void exc2() {
  panic("Non Maskable Interrupt");
};

void exc3() {
  panic("Breakpoint");
};

void exc4() {
  panic("Overflow");
};

void exc5() {
  panic("Bound Range Exceeded");
};

void exc6() {
  panic("Invalid opcode");
};

void exc7() {
  panic("Device not available");
};

void exc8() {
  panic("Double Fault");
};

void exc9() {
  panic("Coprocessor Segment Overrun");
};

void exc10() {
  panic("Invalid TSS");
};

void exc11() {
  panic("Segment not present");
};

void exc12() {
  panic("Stack Exception");
};

void exc13() {
  panic("General Protection fault");
};

void exc14() {
  panic("Page fault");
};

void exc15() {
  panic("Unknown Interrupt");
};

void exc16() {
  panic("x87 Floating Point Exception");
};

void exc17() {
  panic("Alignment check");
};

void exc18() {
  panic("Machine check");
};

void exc19() {
  panic("SIMD floating point Exception");
};

void exc20() {
  panic("Virtualization Exception");
};

void exc30() {
  panic("Security Exception");
};
