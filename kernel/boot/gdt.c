#include "gdt.h"

gdt_desc_t gdt[8];
gdt_pointer_t gdtr = {.size = sizeof(gdt) - 1, .addr = (uint64_t)gdt};

void gdt_init() {
  gdt[1] = (gdt_desc_t){.access = 0b10011010, .granularity = 0b00100000};
  gdt[2] = (gdt_desc_t){.access = 0b10010010, .granularity = 0};
  asm volatile("lgdt %0"
               :
               : "m"(gdtr)
               : "memory");
  asm volatile(
      "mov %%rsp, %%rax\n"
      "push $0x10\n"
      "push %%rax\n"
      " pushf\n"
      " push $0x8\n"
      " push $1f\n"
      " iretq\n"
      " 1:\n"
      " mov $0x10, %%ax\n"
      "  mov %%ax, %%ds\n"
      " mov %%ax, %%es\n"
      "mov %%ax, %%ss\n"
      "mov %%ax, %%fs\n"
      "mov %%ax, %%gs\n"

      :
      :
      : "rax", "memory");
  logf("Loaded GDT.");
}
