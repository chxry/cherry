#pragma once
#include <stdint.h>

typedef struct {
  uint64_t rip;
  uint64_t cs;
  uint64_t rflags;
  uint64_t rsp;
  uint64_t ss;
} __attribute__((__packed__)) frame_t;

#include "io/serial.h"
#include "io/io.h"
#include "io/kb.h"
#include "sys/pit.h"
#include "sys/panic.h"
#include "user/syscall.h"

#define PIC1 0x20
#define PIC2 0xA0
#define ICW1 0x11
#define ICW4 0x01

typedef struct {
  uint16_t offset_lo;
  uint16_t selector;
  uint8_t ist;
  uint8_t type_attr;
  uint16_t offset_mid;
  uint32_t offset_hi;
  uint32_t _;
} __attribute__((packed)) idt_desc_t;

typedef struct {
  uint16_t size;
  uint64_t addr;
} __attribute__((packed)) idt_pointer_t;

void pic_remap(uint8_t pic1, uint8_t pic2);
void idt_init();
idt_desc_t idt_make_entry(uint64_t offset);
