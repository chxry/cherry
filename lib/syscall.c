#include "syscall.h"
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"

uint64_t syscall(uint64_t n) {
  asm volatile("int $0x80");
}

uint64_t syscall1(uint64_t a, uint64_t n) {
  asm volatile("int $0x80");
}

uint64_t syscall2(uint64_t a, uint64_t b, uint64_t n) {
  asm volatile("int $0x80");
}

#pragma clang diagnostic pop