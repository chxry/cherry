#include "boot.h"

bootinfo_t bootinfo;

void init() {
  serial_init();
  gdt_init();
  pic_remap(0x20, 0x28);
  idt_init();
  pit_init(1000);
  bootinfo.mem_usable = mm_init();

  uint8_t table[512];
  ata_read_sectors(table,2,1);
  gpt_entry_t* entry = (gpt_entry_t*)table;
  fs_init(entry->start);
  fb_init();

  int eax, ebx, ecx, edx;
  __cpuid(0, eax, ebx, ecx, edx);
  memcpy(bootinfo.cpu_vendor, &ebx, 4);
  memcpy(bootinfo.cpu_vendor + 4, &edx, 4);
  memcpy(bootinfo.cpu_vendor + 8, &ecx, 4);
  bootinfo.cpu_vendor[12] = '\0';

  logf("cherryos %s booted!", __cherry_version__);
  logf("copyright (c) chxry 2022.");
}
