#pragma once
#include <stdarg.h>
#include <stb/stb_sprintf.h>

#include "boot/boot.h"
#include "io/video.h"
#include "fs/fs.h"
#include "user/prog.h"
#include "lib/mem.h"
#include "lib/string.h"

extern char input_buf[512];

typedef struct {
  char* name;
  char* desc;
  void (*func)(char* args);
} command_t;

void help_command(char* args);
void time_command(char* args);
void fetch_command(char* args);
void cd_command(char* args);
void ls_command(char* args);
void test_command(char* args);

#define WHITE_ "\e\x01"
#define LIGHTGRAY_ "\e\x02"
#define GRAY_ "\e\x03"
#define DARKGRAY_ "\e\x04"
#define BLACK_ "\e\x05"
#define RED_ "\e\x06"
#define ORANGE_ "\e\x07"
#define YELLOW_ "\e\x08"
#define LIME_ "\e\x09"
#define GREEN_ "\e\x0a"
#define GREENBLUE_ "\e\x0b"
#define CYAN_ "\e\x0c"
#define AZURE_ "\e\x0d"
#define BLUE_ "\e\x0e"
#define PURPLE_ "\e\x0f"
#define MAGENTA_ "\e\x10"
#define PINK_ "\e\x10"

void term_init();
void term_clear();
void term_print(char* str);
void term_printf(char* fmt, ...);
void term_render(uint16_t x, uint16_t y, uint16_t w, uint16_t h);
void term_run();
