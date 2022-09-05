#pragma once
#include <stdint.h>

#include "syscall.h"

typedef struct {
  uint8_t second;
  uint8_t minute;
  uint8_t hour;
  uint8_t day;
  uint8_t month;
  uint16_t year;
} time_t;

#define __time(ptr) (void)syscall1((uint64_t)ptr,4)

time_t time();
