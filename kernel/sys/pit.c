#include "pit.h"

uint64_t ticks = 0;

void pit_init(uint16_t hz) {
  uint32_t divisor = PIT_FREQ / hz;
  outb(0x43, 0x36);
  outb(0x40, divisor);
  outb(0x40, divisor >> 8);
}

void __attribute__((interrupt)) pit_handler(void* frame) {
  ticks++;
  outb(0x20, 0x20);
}

void sleep(uint64_t ms) {
  uint64_t t = ticks + ms;
  while (ticks < t)
    ;
}
