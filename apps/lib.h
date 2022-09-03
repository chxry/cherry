#pragma once
#include <stdint.h>
#include <stdarg.h>

int main();
int _start() {
  return main();
}

#define STB_SPRINTF_IMPLEMENTATION
#include <stb/stb_sprintf.h>

#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wreturn-type"
uint64_t syscall(uint64_t n) {
  asm volatile("int $0x80");
}

uint64_t syscall1(uint64_t a,uint64_t n) {
  asm volatile("int $0x80");
}

uint64_t syscall2(uint64_t a, uint64_t b, uint64_t n) {
  asm volatile("int $0x80");
}
#pragma clang diagnostic pop

#define print(str) syscall1(str,1)
#define malloc(len) syscall1(len,2)
#define free(ptr) syscall1(ptr,3)

void printf(char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  char buf[512];
  stbsp_vsnprintf(buf, sizeof(buf), fmt, va);
  print(buf);
  va_end(va);
}
