#include <stdint.h>
#include <stb/stb_sprintf.h>

#include "boot/boot.h"
#include "io/video.h"
#include "sys/time.h"
#include "lib/mem.h"
#include "lib/string.h"

char* term_buf;
char input_buf[512] = "";

void term_init() {
  term_buf = malloc(1);
  term_buf[0] = 0;
}

void term_clear() {
  free(term_buf);
  term_init();
}

void term_print(char* str) {
  char* new = malloc(strlen(term_buf) + strlen(str) + 1);
  memcpy(new, term_buf, strlen(term_buf));
  memcpy(new + strlen(term_buf), str, strlen(str));
  new[strlen(term_buf) + strlen(str)] = 0;
  free(term_buf);
  term_buf = new;
}

void term_printf(char* fmt, ...) {
  va_list va;
  va_start(va, fmt);
  char buf[512];
  stbsp_vsnprintf(buf, sizeof(buf), fmt, va);
  term_print(buf);
  va_end(va);
}

void term_render() {
  uint16_t x = 16;
  uint16_t y = 16;
  color_t color = WHITE;
  draw_rect(0, 0, fb->width, fb->height, (color_t){8,8,8});
  for (uint32_t i = 0; term_buf[i]; i++) {
    switch (term_buf[i]) {
    case '\n':
      y += font.height;
      break;
    case '\r':
      x = 16;
      break;
    case '\e':
      i++;
      switch (term_buf[i]) {
      case '0':
        color = WHITE;
        break;
      case '1':
        color = RED;
        break;
      case '2':
        color = GREEN;
        break;
      case '3':
        color = BLUE;
        break;
      }
      break;
    default:
      draw_char(x, y, term_buf[i], color);
      x += font.width;
    }
  }
}

void _start(void) {
  init();
  term_init();

  for (;;) {
  term_clear();
    time_t time = get_time();
    term_print("\e1\3\e0 floppa os! \e1\3\e0\n\r");
    term_printf("%02i:%02i:%02i | %02i/%02i/%i\n\r", time.hour, time.minute, time.second,time.day,time.month,time.year);
    term_printf(">%s_",input_buf);
    term_render();
    fb_render();
    sleep(10);
  }
}
