#include "uart.h"

volatile char* uart = (char*)0x10000000;

void uart_write_char(char c) {
  *uart = c;
}

void uart_write_str(const char* s) {
  while (*s)
    uart_write_char(*s++);
}
