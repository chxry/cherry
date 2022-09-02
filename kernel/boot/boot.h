#pragma once
#include <stdint.h>
#include <cpuid.h>

#include "sys/interrupts.h"
#include "sys/pit.h"
#include "sys/mm.h"
#include "fs/fs.h"
#include "io/serial.h"
#include "io/video.h"
#include "io/ata.h"
#include "lib/mem.h"
#include "gdt.h"

typedef struct {
  uint64_t type[2];
  uint64_t guid[2];
  uint64_t start;
  uint64_t end;
  uint64_t attr;
  uint16_t name[36];
} __attribute__((packed)) gpt_entry_t;

typedef struct {
  uint64_t mem_usable;
  char cpu_vendor[13];
  uint64_t boot_ticks;
} bootinfo_t;

extern bootinfo_t bootinfo;

void init();
