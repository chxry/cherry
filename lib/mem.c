#include "mem.h"

void memcpy(void* dest, void* src, uint64_t len) {
  char* d = dest;
  char* s = src;
  while (len--)
    *d++ = *s++;
}