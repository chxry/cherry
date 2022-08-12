#pragma once
#include <stdint.h>

#include "io/serial.h"
#include "io/ata.h"
#include "lib/mem.h"
#include "lib/string.h"

typedef struct {
  uint32_t _;
  char signature[8];
  uint64_t total_blocks;
  uint64_t maindir_blocks;
  uint64_t bytes_per_block;
  uint32_t __;
  uint64_t uuid[2];
} __attribute__((packed)) echfs_identity_t;

typedef struct {
  uint64_t parent_id;
  uint8_t type;
  char name[201];
  uint64_t atime;
  uint64_t mtime;
  uint16_t perms;
  uint16_t owner;
  uint16_t group;
  uint64_t ctime;
  uint64_t block;
  uint64_t size;
} __attribute__((packed)) echfs_entry_t;

#define ROOT_ID 0xffffffffffffffff

void fs_init(uint64_t s);
echfs_entry_t* fs_search_path(char* path);
echfs_entry_t* fs_search(char* path, uint64_t parent);
char* fs_read(echfs_entry_t* entry);