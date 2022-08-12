#include "mem.h"

void* malloc(uint64_t len) {
  return ta_alloc(len);
}

void* calloc(uint64_t len) {
  return ta_calloc(len,1);
}

void free(void* ptr) {
  ta_free(ptr);
}

void memcpy(void* dest, void* src, uint64_t len) {
  char* d = dest;
  char* s = src;
  while (len--)
    *d++ = *s++;
}