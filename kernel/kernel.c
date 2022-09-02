#include "boot/boot.h"
#include "io/video.h"
#include "apps/term.h"
#include "apps/bar.h"

#include "sys/pit.h"

void _start(void) {
  init();
  term_init();
  term_print("Welcome to cherry os!\n\r");
  term_printf("Booted in %lums.\n\r", bootinfo.boot_ticks);

  image_t* img = (image_t*)fs_read(fs_search_path("root/wallpaper.img"));

  for (;;) {
    draw_image(0, 0, img);
    bar_render();
    term_render(16, 32, fb->width - 32, fb->height - 32);
    fb_render();
    sleep(10);
  }
}
