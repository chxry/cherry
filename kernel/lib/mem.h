#pragma once
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>

void* memset(void* buf, uint8_t c, size_t n);
bool strcmp(const char* a, const char* b);
size_t strlen(const char* str);
uint32_t bswap32(uint32_t x);
size_t align_up(size_t ptr, size_t align);
