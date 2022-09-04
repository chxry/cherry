#include "mm.h"

volatile struct limine_memmap_request mmap_rqeuest = {
    .id = LIMINE_MEMMAP_REQUEST,
    .revision = 0};

void* kmalloc(uint64_t len) {
  return ta_alloc(len);
}

void* kcalloc(uint64_t len) {
  return ta_calloc(len, 1);
}

void kfree(void* ptr) {
  ta_free(ptr);
}

uint64_t mm_init() {
  int l = 0;
  for (uint64_t i = 0; i < mmap->entry_count; i++) {
    struct limine_memmap_entry* entry = mmap->entries[i];
    if (entry->type == LIMINE_MEMMAP_USABLE && entry->length > mmap->entries[l]->length) {
      l = i;
    }
  }
  struct limine_memmap_entry* largest = mmap->entries[l];
  ta_init((void*)largest->base, largest->base + largest->length, 256, 16, 8);
  return largest->length;
}