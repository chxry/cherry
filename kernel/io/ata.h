#pragma once
#include <stdint.h>

#include "io.h"
#include "sys/interrupts.h"

#define STATUS_BSY 0x80
#define STATUS_RDY 0x40
#define STATUS_DRQ 0x08
#define STATUS_DF 0x20
#define STATUS_ERR 0x01

void __attribute__((interrupt)) ata_handler(frame_t* frame);
void ata_read_sectors(uint8_t* buf, uint32_t block, uint8_t sector_count);
void ata_write_sectors(uint8_t* buf, uint32_t block, uint8_t sector_count);
