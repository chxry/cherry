#pragma once
#include <stdint.h>
#include <limine/limine.h>
#include <tinyalloc/tinyalloc.h>

void* kmalloc(uint64_t len);
void* kcalloc(uint64_t len);
void kfree(void* ptr);
uint64_t mm_init();

#define mmap mmap_rqeuest.response
