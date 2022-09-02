#pragma once
#include <stdarg.h>
#include <stb/stb_sprintf.h>

#include "boot/boot.h"
#include "io/video.h"
#include "sys/time.h"
#include "sys/pit.h"
#include "fs/fs.h"
#include "lib/mem.h"
#include "lib/string.h"

extern char input_buf[512];

typedef struct {
  char* name;
  void (*func)(char* args);
} command_t;

void term_init();
void term_clear();
void term_print(char* str);
void term_printf(char* fmt, ...);
void term_render(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void term_run();
