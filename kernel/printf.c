#include "printf.h"
#include "uart.h"

#define STB_SPRINTF_IMPLEMENTATION
#include "stb_sprintf.h"

void kprintf(const char * fmt, ...) {
  va_list va;
  va_start(va, fmt);
  char buf[512];
  stbsp_vsnprintf(buf, sizeof(buf), fmt, va);
  uart_write_str(buf);
  va_end(va);
}
