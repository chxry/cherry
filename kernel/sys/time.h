#pragma once
#include <stdint.h>

#include "io/io.h"

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t month;
  uint16_t year;
} time_t;

time_t get_time();
