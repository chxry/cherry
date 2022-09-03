#include "bar.h"

void bar_render() {
  time_t time = get_time();
  draw_rect(0, 0, fb->width, 16, DARK_GRAY);
  draw_char(8, 0, 3, RED);
  draw_str(24, 0, "cherry os "__cherry_version__, WHITE);
  char time_buf[20];
  stbsp_snprintf(time_buf, sizeof(time_buf), "%02i:%02i:%02i %02i/%02i/%i\n\r", time.hour, time.minute, time.second, time.day, time.month, time.year);
  draw_str(fb->width - 160, 0, time_buf, WHITE);
}
