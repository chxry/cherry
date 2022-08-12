#pragma once
#include <stdint.h>
#include <limine/limine.h>

#include "lib/mem.h"

#define WHITE \
  (color_t) { .r = 255, .g = 255, .b = 255, .a = 255 }
#define BLACK \
  (color_t) { .r = 0, .g = 0, .b = 0, .a = 255 }
#define RED \
  (color_t) { .r = 255, .g = 0, .b = 0, .a = 255 }
#define GREEN \
  (color_t) { .r = 0, .g = 255, .b = 0, .a = 255 }
#define BLUE \
  (color_t) { .r = 0, .g = 0, .b = 255, .a = 255 }

typedef struct {
  uint8_t b;
  uint8_t g;
  uint8_t r;
  uint8_t a;
} __attribute__((packed)) color_t;

typedef struct {
  uint32_t magic;
  uint32_t version;
  uint32_t headersize;
  uint32_t flags;
  uint32_t numglyph;
  uint32_t glyph_size;
  uint32_t height;
  uint32_t width;
  uint8_t data[];
} __attribute__((packed)) psf_t;

extern volatile struct limine_framebuffer_request fb_request;
#define fb fb_request.response->framebuffers[0]

extern psf_t font;

void fb_init();
void draw_px(uint16_t x, uint16_t y, color_t color);
void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, color_t color);
void draw_char(uint16_t x, uint16_t y, char c, color_t color);
void draw_str(uint16_t x, uint16_t y, char* str, color_t color);
void fb_render();
