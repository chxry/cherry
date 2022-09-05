#pragma once
#include <stdint.h>

#include "syscall.h"

#define malloc(len) (void*)syscall1(len, 2)
#define free(ptr) (void*)syscall1((uint64_t)ptr, 3)

void memcpy(void* dest, void* src, uint64_t len);
