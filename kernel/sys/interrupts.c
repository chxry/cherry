#include "interrupts.h"

idt_desc_t idt[256];
idt_pointer_t idtr = {.size = 256 * sizeof(idt_desc_t), .addr = (uint64_t)idt};

extern void irq0();
extern void irq1();
extern void irq14();

void pic_remap(uint8_t pic1, uint8_t pic2) {
  outb(PIC1, ICW1);
  outb(PIC2, ICW1);

  outb(PIC1 + 1, pic1);
  outb(PIC2 + 1, pic2);

  outb(PIC1 + 1, 4);
  outb(PIC2 + 1, 2);

  outb(PIC1 + 1, ICW4);
  outb(PIC2 + 1, ICW4);
  logf("Remapped PIC.");
}

void idt_init() {
  idt[0] = idt_make_entry((uint64_t)&exc0);
  idt[1] = idt_make_entry((uint64_t)&exc1);
  idt[2] = idt_make_entry((uint64_t)&exc2);
  idt[3] = idt_make_entry((uint64_t)&exc3);
  idt[4] = idt_make_entry((uint64_t)&exc4);
  idt[5] = idt_make_entry((uint64_t)&exc5);
  idt[6] = idt_make_entry((uint64_t)&exc6);
  idt[7] = idt_make_entry((uint64_t)&exc7);
  idt[8] = idt_make_entry((uint64_t)&exc8);
  idt[10] = idt_make_entry((uint64_t)&exc10);
  idt[11] = idt_make_entry((uint64_t)&exc11);
  idt[12] = idt_make_entry((uint64_t)&exc12);
  idt[13] = idt_make_entry((uint64_t)&exc13);
  idt[14] = idt_make_entry((uint64_t)&exc14);
  idt[15] = idt_make_entry((uint64_t)&exc15);
  idt[16] = idt_make_entry((uint64_t)&exc16);
  idt[17] = idt_make_entry((uint64_t)&exc17);
  idt[18] = idt_make_entry((uint64_t)&exc18);
  idt[19] = idt_make_entry((uint64_t)&exc19);
  idt[20] = idt_make_entry((uint64_t)&exc20);
  idt[30] = idt_make_entry((uint64_t)&exc30);
  idt[32] = idt_make_entry((uint64_t)&irq0);
  idt[33] = idt_make_entry((uint64_t)&irq1);
  idt[46] = idt_make_entry((uint64_t)&irq14);

  asm volatile("lidt %0"
               :
               : "m"(idtr));
  asm volatile("sti");
  logf("Loaded IDT.");
}

idt_desc_t idt_make_entry(uint64_t offset) {
  return (idt_desc_t){
      .selector = 0x08,
      .offset_lo = offset & 0xFFFF,
      .offset_mid = (offset >> 16) & 0xFFFF,
      .offset_hi = (offset >> 32) & 0xFFFFFFFF,
      .ist = 0,
      .zero = 0,
      .type_attr = 0x8e};
}
