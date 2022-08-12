#pragma once
#include <stdint.h>
#include <tinyalloc/tinyalloc.h>

void* malloc(uint64_t len);
void* calloc(uint64_t len);
void free(void* ptr);
void memcpy(void* dest, void* src, uint64_t len);
