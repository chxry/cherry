#pragma once
#include <stdint.h>

#include "io/serial.h"

typedef struct {
  uint16_t limit_low16;
  uint16_t base_low16;
  uint8_t base_mid8;
  uint8_t access;
  uint8_t granularity;
  uint8_t base_high8;
} __attribute__((packed)) gdt_desc_t;

typedef struct {
  uint16_t size;
  uint64_t addr;
} __attribute__((packed)) gdt_pointer_t;

void gdt_init();
