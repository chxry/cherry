#include "ata.h"

void ata_waitbsy() {
  while (inb(0x1F7) & STATUS_BSY)
    ;
}
void ata_waitdrq() {
  while (!(inb(0x1F7) & STATUS_RDY))
    ;
}

void ata_handler() {
  outb(0x20, 0x20);
}

void ata_read_sectors(uint8_t* buf, uint32_t block, uint8_t sectors) {
  ata_waitbsy();

  outb(0x1F6, 0xE0 | ((block >> 24) & 0xF));
  outb(0x1F2, sectors);
  outb(0x1F3, (uint8_t)block);
  outb(0x1F4, (uint8_t)(block >> 8));
  outb(0x1F5, (uint8_t)(block >> 16));
  outb(0x1F7, 0x20);

  uint16_t* target = (uint16_t*)buf;

  for (int j = 0; j < sectors; j++) {
    ata_waitbsy();
    ata_waitdrq();

    for (int i = 0; i < 256; i++) {
      target[i] = inw(0x1F0);
    }
    target += 256;
  }
}

void ata_write_sectors(uint8_t* buf, uint32_t block, uint8_t sectors) {
  ata_waitbsy();

  outb(0x1F6, 0xE0 | ((block >> 24) & 0xF));
  outb(0x1F2, sectors);
  outb(0x1F3, (uint8_t)block);
  outb(0x1F4, (uint8_t)(block >> 8));
  outb(0x1F5, (uint8_t)(block >> 16));
  outb(0x1F7, 0x30);

  uint16_t* target = (uint16_t*)buf;

  for (int j = 0; j < sectors; j++) {
    ata_waitbsy();
    ata_waitdrq();

    for (int i = 0; i < 256; i++) {
      outw(0x1F0, buf[i]);
    }
  }
}