#pragma once
#include <stdint.h>
#include <stdarg.h>
#include <stb/stb_sprintf.h>

#include "syscall.h"

#define print(str) syscall1(str, 1)

void printf(char* fmt, ...);