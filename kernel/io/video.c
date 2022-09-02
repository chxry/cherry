#include "video.h"

volatile struct limine_framebuffer_request fb_request = {
    .id = LIMINE_FRAMEBUFFER_REQUEST,
    .revision = 0};

psf_t* font;
color_t* fbuf;

void fb_init() {
  font = (psf_t*)fs_read(fs_search_path("root/goha16.psfu"));
  fbuf = calloc(fb->width * fb->height * sizeof(color_t));
}

void draw_px(uint16_t x, uint16_t y, color_t color) {
  fbuf[x + y * fb->width] = color;
}

void draw_rect(uint16_t x, uint16_t y, uint16_t w, uint16_t h, color_t color) {
  for (uint16_t yy = y; yy < y + h; yy++) {
    for (uint16_t xx = x; xx < x + w; xx++) {
      draw_px(xx, yy, color);
    }
  }
}

void draw_char(uint16_t x, uint16_t y, char c, color_t color) {
  color_t* buf = fb->address;
  uint8_t* glyph = &font->data[c * font->glyph_size];
  uint8_t masks[8] = {128, 64, 32, 16, 8, 4, 2, 1};

  for (uint32_t yy = 0; yy < font->height; yy++) {
    for (uint32_t xx = 0; xx < font->width; xx++) {
      if (glyph[yy] & masks[xx]) {
        draw_px(x + xx, y + yy, color);
      }
    }
  }
}

void draw_str(uint16_t x, uint16_t y, char* str, color_t color) {
  for (uint16_t i = 0; str[i]; i++) {
    draw_char(x + i * font->width, y, str[i], color);
  }
}

void draw_image(uint16_t x, uint16_t y, image_t* img) {
  uint64_t i = 0;
  for (uint16_t yy = y; yy < y + img->h; yy++) {
    for (uint16_t xx = x; xx < x + img->w; xx++) {
      draw_px(xx, yy, (&img->data)[i]);
      i++;
    }
  }
}

void fb_render() {
  memcpy(fb->address, fbuf, fb->width * fb->height * sizeof(color_t));
}