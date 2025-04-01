#include "printf.h"
#include "sbi.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

void write_char(char c) {
  sbi_call(c, 0, 0, 0, 0, 0, 0, 1);
}

void write_str(const char* s) {
  while(*s)
    write_char(*s++);
}

void kprintf(const char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  char buf[512];
  stbsp_vsnprintf(buf, sizeof(buf), fmt, va);
  write_str(buf);
  va_end(va);
}
