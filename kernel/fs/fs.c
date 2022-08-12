#include "fs.h"

echfs_entry_t* entries;
uint64_t start;

void fs_init(uint64_t s) {
  start = s;
  uint8_t table[512];
  ata_read_sectors(table,start,1);
  echfs_identity_t* identity = (echfs_identity_t*)table;
  entries = malloc(identity->maindir_blocks*512);
  ata_read_sectors((uint8_t*)entries,start+17+identity->total_blocks/64,identity->total_blocks);
  
  logf("root/test/file.txt: %s",fs_read(fs_search_path("root/test/file.txt")));
}

echfs_entry_t* fs_search_path(char* path) {
  uint64_t parent = ROOT_ID;
  char* tmp = malloc(strlen(path));
  uint64_t n;
  while((n = strfind(path,'/'))) {
    memcpy(tmp,path,n);
    parent = fs_search(tmp,parent)->block;
    path+=n+1;
  }
  free(tmp);
  return fs_search(path,parent);
}

echfs_entry_t* fs_search(char* path, uint64_t parent) {
  for (int i = 0;;i++) {
    echfs_entry_t* entry = entries+i;
    if (!entry->parent_id) {
      return 0;
    }
    if (entry->parent_id == parent && strcmp(entry->name,path)) {
      return entry;
    }
  }
}

char* fs_read(echfs_entry_t* entry) {
  uint64_t sectors = entry->size/512+1;
  uint8_t* entries = malloc(sectors*512);
  ata_read_sectors(entries,start+entry->block,sectors);
  char* buf = malloc(entry->size);
  memcpy(buf,entries,entry->size);
  free(entries);
  return buf;
}