#include "io.h"

uint8_t inb(uint16_t port) {
  uint8_t val;
  asm volatile("inb %1, %0"
               : "=a"(val)
               : "Nd"(port));
  return val;
}

void outb(uint16_t port, uint8_t val) {
  asm volatile("outb %0, %1"
               :
               : "a"(val), "Nd"(port));
}

uint16_t inw(uint16_t port) {
  uint16_t val;
  asm("in %%dx, %%ax"
      : "=a"(val)
      : "d"(port));
  return val;
}

void outw(uint16_t port, uint16_t val) {
  asm("out %%ax, %%dx"
      :
      : "a"(val), "d"(port));
}

uint32_t inl(uint16_t port) {
  uint32_t val;
  asm volatile("in %%dx,%%eax"
               : "=a"(val)
               : "d"(port));
  return val;
}

void outl(uint16_t port, uint32_t val) {
  asm volatile("out %%eax,%%dx" ::"a"(val), "d"(port));
}
