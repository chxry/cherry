#pragma once
#include <stdarg.h>
#include <stb/stb_sprintf.h>

#include "io.h"

#define PORT 0x3f8

void serial_init();
void serial_putc(char c);
void serial_putstr(char* str);
void logf(char* fmt, ...);
