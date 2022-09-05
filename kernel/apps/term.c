#include "term.h"

char* term_buf;
char term_dir[512] = "root";
char input_buf[512] = "";
echfs_entry_t** apps;

void term_init() {
  term_buf = kmalloc(1);
  term_buf[0] = 0;
  apps = fs_list(fs_search_path("apps"));
}

void term_clear() {
  kfree(term_buf);
  term_buf = kmalloc(1);
  term_buf[0] = 0;
}

void term_print(char* str) {
  char* new = kmalloc(strlen(term_buf) + strlen(str) + 1);
  memcpy(new, term_buf, strlen(term_buf));
  memcpy(new + strlen(term_buf), str, strlen(str));
  new[strlen(term_buf) + strlen(str)] = 0;
  kfree(term_buf);
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
  uint16_t start = x;
  color_t color = WHITE;
  for (uint32_t i = 0; term_buf[i]; i++) {
    switch (term_buf[i]) {
    case '\n':
      y += font->height;
      break;
    case '\r':
      x = start;
      break;
    case '\e':
      i++;
      switch (term_buf[i]) {
      case 1:
        color = WHITE;
        break;
      case 2:
        color = LIGHT_GRAY;
        break;
      case 3:
        color = GRAY;
        break;
      case 4:
        color = DARK_GRAY;
        break;
      case 5:
        color = BLACK;
        break;
      case 6:
        color = RED;
        break;
      case 7:
        color = ORANGE;
        break;
      case 8:
        color = YELLOW;
        break;
      case 9:
        color = LIME;
        break;
      case 10:
        color = GREEN;
        break;
      case 11:
        color = GREEN_BLUE;
        break;
      case 12:
        color = CYAN;
        break;
      case 13:
        color = AZURE;
        break;
      case 14:
        color = BLUE;
        break;
      case 15:
        color = PURPLE;
        break;
      case 16:
        color = MAGENTA;
        break;
      case 17:
        color = PINK;
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

command_t cmds[] = {
    {"help", "show a list of commands", help_command},
    {"fetch", "show system info", fetch_command},
    {"cd", "change directory", cd_command},
    {"ls", "list files", ls_command}};

void help_command(char* args) {
  term_print("commands:\n\r");
  for (int i = 0; i < sizeof(cmds) / sizeof(command_t); i++) {
    term_printf(WHITE_ "    %-6s " GRAY_ "%s.\n\r", cmds[i].name, cmds[i].desc);
  }
}

void fetch_command(char* args) {
  term_print("                     root@cherry\n\r");
  term_print(GREEN_ "  __.--~~.,-.__      " WHITE_ "os      " GRAY_ "cherry "__cherry_version__
                    "\n\r");
  term_printf(GREEN_ "  `~-._.-(`-.__`-.   " WHITE_ "cpu     " GRAY_ "%s\n\r", bootinfo.cpu_vendor);
  term_printf(GREEN_ "          \\    `~~`  " WHITE_ "mem     " GRAY_ "%luMB\n\r", bootinfo.mem_usable / 1024 / 1024);
  term_print(RED_ "     .--." GREEN_ "/ \\         " WHITE_ "uptime  " GRAY_);
  uint64_t minutes = ticks / 60000;
  uint64_t seconds = ticks / 1000 - minutes * 60;
  if (minutes) {
    term_printf("%lum%lus\n\r", minutes, seconds);
  } else {
    term_printf("%lus\n\r", seconds);
  }
  term_printf(RED_ "    /#   \\  " GREEN_ "\\" RED_ ".--.    " WHITE_ "display " GRAY_ "%ix%i\n\r", fb->width, fb->height);
  term_printf(RED_ "    \\    /  /#   \\   " WHITE_ "disk   " GRAY_ "%luMB\n\r", bootinfo.disk_size / 1024 / 1024);
  term_print(RED_ "     '--'   \\    /   \n\r");
  term_print(RED_ "             '--'    \n\r");
}

void cd_command(char* args) {
  if (args) {
    // verify
    memcpy(term_dir, args, strlen(args));
  } else {
    term_print("cd: no directory provided.\n\r");
  }
}

void ls_command(char* args) {
  echfs_entry_t** entries = fs_list(fs_search_path(args ? args : term_dir));
  for (int i = 0; entries[i]; i++) {
    term_printf("%s ", entries[i]->name);
  }
  term_print("\n\r");
}

void term_run() {
  term_printf(WHITE_ "%s>%s\n\r", term_dir, input_buf);
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
  for (int i = 0; apps[i]; i++) {
    if (strcmp(input_buf, apps[i]->name)) {
      prog_exec_file(apps[i]);
      goto end;
    }
  }
  if (input_buf[0]) {
    term_printf("%s: command not found.\n\r", input_buf);
  }
end:
  input_buf[0] = 0;
}