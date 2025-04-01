#include "mem.h"

void* memset(void* buf, uint8_t c, size_t n) {
  uint8_t* p = (uint8_t*)buf;
  while (n--)
    *p++ = c;
  return buf;
}

bool strcmp(const char* a, const char* b) {
  while (*a) {
    if (*a++ != *b++) {
      return false;
    }
  }
  return !*b;
}

size_t strlen(const char* str) {
  size_t n = 0;
  while (str[n++]);
  return n;
}

uint32_t bswap32(uint32_t x) {
  return ((x & 0x000000FF) << 24) | ((x & 0x0000FF00) << 8) | ((x & 0x00FF0000) >> 8) | ((x & 0xFF000000) >> 24);
}

void* align_up(void* ptr, size_t align) {
  return (void*)((size_t)(ptr + align - 1) & ~(align - 1));
}
