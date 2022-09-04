#pragma once
#include <stdint.h>

#include "syscall.h"

#define malloc(len) syscall1(len, 2)
#define free(ptr) syscall1(ptr, 3)

void memcpy(void* dest, void* src, uint64_t len);
