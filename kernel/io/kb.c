#include "kb.h"

extern char input_buf[128];

bool caps = false;
uint8_t scancodes[] = {
    0, 0, '1', '2',
    '3', '4', '5', '6',
    '7', '8', '9', '0',
    '-', '=', 0, 0,
    'q', 'w', 'e', 'r',
    't', 'y', 'u', 'i',
    'o', 'p', '[', ']',
    0, 0, 'a', 's',
    'd', 'f', 'g', 'h',
    'j', 'k', 'l', ';',
    '\'', '`', 0, '\\',
    'z', 'x', 'c', 'v',
    'b', 'n', 'm', ',',
    '.', '/', 0, '*',
    0, ' '};


void kb_handler() {
  uint8_t chr = inb(0x60);
  if (chr < 0x3A) {
    uint8_t scancode = scancodes[chr];
    int n = strlen(input_buf);
    input_buf[n] = caps ? scancode - 32 : scancode;
    input_buf[n+1] = 0;
  } else {
    switch (chr) {
    case 0x9c: // enter
      break;
    case 0x8E: // backspace
      input_buf[strlen(input_buf)-1] = 0;
      break;
    case 0xBA: // caps
      caps = !caps;
      break;
    }
  }
  outb(0x20, 0x20);
}