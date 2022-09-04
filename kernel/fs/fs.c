#include "fs.h"

echfs_entry_t* entries;
uint64_t start;

void fs_init(uint64_t s) {
  start = s;
  uint8_t table[512];
  ata_read_sectors(table, start, 1);
  echfs_identity_t* identity = (echfs_identity_t*)table;
  entries = kmalloc(identity->maindir_blocks * 512);
  ata_read_sectors((uint8_t*)entries, start + 17 + identity->total_blocks / 64, identity->total_blocks);
}

echfs_entry_t* fs_search_path(char* path) {
  uint64_t parent = ROOT_ID;
  char* tmp = kmalloc(strlen(path));
  uint64_t n;
  while ((n = strfind(path, '/'))) {
    memcpy(tmp, path, n);
    parent = fs_search(tmp, parent)->block;
    path += n + 1;
  }
  kfree(tmp);
  return fs_search(path, parent);
}

echfs_entry_t* fs_search(char* path, uint64_t parent) {
  for (int i = 0;; i++) {
    echfs_entry_t* entry = entries + i;
    if (!entry->parent_id) {
      return 0;
    }
    if (entry->parent_id == parent && strcmp(entry->name, path)) {
      return entry;
    }
  }
}

char* fs_read(echfs_entry_t* file) {
  uint64_t sectors = file->size / 512 + 1;
  uint8_t* entries = kmalloc(sectors * 512);
  int i = 0;
  while (i < sectors / 255) {
    ata_read_sectors(entries + i * 255 * 512, start + file->block + i * 255, 255);
    i++;
  }
  ata_read_sectors(entries + i * 255 * 512, start + file->block + i * 255, sectors - i * 255);
  char* buf = kmalloc(file->size);
  memcpy(buf, entries, file->size);
  kfree(entries);
  return buf;
}

echfs_entry_t** fs_list(echfs_entry_t* dir) {
  echfs_entry_t** tmp = kmalloc(64 * sizeof(echfs_entry_t**));
  int n = 0;
  for (int i = 0;; i++) {
    echfs_entry_t* entry = entries + i;
    if (!entry->parent_id) {
      break;
    }
    if (entry->parent_id == dir->block) {
      tmp[n] = entry;
      n++;
    }
  }
  uint64_t len = (n + 1) * sizeof(echfs_entry_t**);
  echfs_entry_t** list = kmalloc(len);
  memcpy(list, tmp, len);
  list[n] = 0;
  kfree(tmp);
  return list;
}