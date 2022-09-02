#include "term.h"

char* term_buf;
char term_dir[512] = "root";
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

void term_render(uint16_t x, uint16_t y, uint16_t w, uint16_t h) {
  color_t color = WHITE;
  // draw_rect(16, 16, w, h, BLACK);
  for (uint32_t i = 0; term_buf[i]; i++) {
    switch (term_buf[i]) {
    case '\n':
      y += font->height;
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
      case '4':
        color = GRAY;
        break;
      }
      break;
    default:
      draw_char(x, y, term_buf[i], color);
      x += font->width;
    }
  }
  char buf[512];
  stbsp_snprintf(buf, sizeof(buf), "%s>%s_", term_dir, input_buf);
  draw_str(x, y, buf, WHITE);
}

void help_command(char* args) {
  term_print("commands: help, time, fetch\n\r");
}

void time_command(char* args) {
  time_t time = get_time();
  term_printf("%02i:%02i:%02i %02i/%02i/%i\n\r", time.hour, time.minute, time.second, time.day, time.month, time.year);
}

void fetch_command(char* args) {
  term_print("                     root@cherry\n\r");
  term_printf("\e2  __.--~~.,-.__      \e0os     \e4cherry %s\n\r", __cherry_version__);
  term_printf("\e2  `~-._.-(`-.__`-.   \e0cpu    \e4%s\n\r", bootinfo.cpu_vendor);
  term_printf("\e2          \\    `~~`  \e0mem    \e4%luMB\n\r", bootinfo.mem_usable / 1024 / 1024);
  term_print("\e1     .--.\e2/ \\         \e0uptime \e4");
  uint64_t minutes = ticks / 60000;
  uint64_t seconds = ticks / 1000 - minutes * 60;
  if (minutes) {
    term_printf("%lum%lus\n\r", minutes, seconds);
  } else {
    term_printf("%lus\n\r", seconds);
  }
  term_print("\e1    /#   \\  \e2\\\e1.--.    \n\r");
  term_print("\e1    \\    /  /#   \\   \n\r");
  term_print("\e1     '--'   \\    /   \n\r");
  term_print("\e1             '--'    \n\r");
}

command_t cmds[] = {
    {"help", help_command},
    {"time", time_command},
    {"fetch", fetch_command},
};

void term_run() {
  term_printf("\e0%s>%s\n\r", term_dir, input_buf);
  uint64_t cmd_end = strfind(input_buf, ' ');
  char* args = 0;
  if (cmd_end) {
    input_buf[cmd_end] = 0;
    args = input_buf + cmd_end + 1;
  }
  for (int i = 0; i < sizeof(cmds) / sizeof(command_t); i++) {
    if (strcmp(input_buf, cmds[i].name)) {
      cmds[i].func(args);
      goto end;
    }
  }
  if (input_buf[0]) {
    term_printf("%s: command not found.\n\r", input_buf);
  }
end:
  input_buf[0] = 0;
}