#include "mem.h"

void* memset(void* buf, uint8_t c, size_t n) {
  uint8_t* p = (uint8_t*)buf;
  while (n--)
    *p++ = c;
  return buf;
}
