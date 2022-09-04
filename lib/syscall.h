#pragma once
#include <stdint.h>
#include <stdarg.h>

uint64_t syscall(uint64_t n);
uint64_t syscall1(uint64_t a, uint64_t n);
uint64_t syscall2(uint64_t a, uint64_t b, uint64_t n);
