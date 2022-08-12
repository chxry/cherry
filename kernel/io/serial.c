#include "serial.h"

void serial_init() {
  outb(PORT + 1, 0x00); // disable interrupts
  outb(PORT + 3, 0x80); // set baud rate divisor
  outb(PORT + 0, 0x03); // set divisor to 3 (lo)
  outb(PORT + 1, 0x00); // set divisor to 3 (hi)
  outb(PORT + 3, 0x03); // 8 bits, no parity, one stop bit
  outb(PORT + 2, 0xC7); // enable fifo
  outb(PORT + 4, 0x0B); // enable irqs
}

void serial_putc(char c) {
  while ((inb(PORT + 5) & 0x20) == 0)
    ;
  outb(PORT, c);
}

void serial_putstr(char* str) {
  while (*str) {
    serial_putc(*str++);
  }
}

void logf(char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  char buf[512];
  stbsp_vsnprintf(buf, sizeof(buf), fmt, va);
  serial_putstr("\033[1minfo: \033[0m");
  serial_putstr(buf);
  serial_putstr("\n");
  va_end(va);
}
