#include "time.h"

uint8_t get_reg(uint8_t reg) {
  outb(0x70, reg);
  return inb(0x71);
}

time_t get_time() {
  while (get_reg(0x0A) & 0x80)
    ;
  uint8_t second = get_reg(0x00);
  uint8_t minute = get_reg(0x02);
  uint8_t hour = get_reg(0x04);
  uint8_t day = get_reg(0x07);
  uint8_t month = get_reg(0x08);
  uint16_t year = get_reg(0x09);
  uint8_t century = get_reg(0x32);
  uint8_t BCD = get_reg(0x0B);

  if (!(BCD & 0x04)) {
    second = (second & 0x0F) + ((second / 16) * 10);
    minute = (minute & 0x0F) + ((minute / 16) * 10);
    hour = ((hour & 0x0F) + (((hour & 0x70) / 16) * 10)) | (hour & 0x80);
    day = (day & 0x0F) + ((day / 16) * 10);
    month = (month & 0x0F) + ((month / 16) * 10);
    year = (year & 0x0F) + ((year / 16) * 10);
    century = (century & 0x0F) + ((century / 16) * 10);
  }

  year += century * 100;

  return (time_t){second, minute, hour, day, month, year};
}
