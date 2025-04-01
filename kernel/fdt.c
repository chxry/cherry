#include "fdt.h"
#include "printf.h"
#include "lib/mem.h"

fdt_header_t* fdt_parse_header(size_t fdt_addr) {
  fdt_header_t* header = (fdt_header_t*)fdt_addr;
  if (bswap32(header->magic) != FDT_MAGIC)
    return 0;
  return header;
}

void fdt_dump(fdt_header_t* header) {
  uint8_t* ptr = (uint8_t*)header + bswap32(header->off_dt_struct);
  char* strings = (char*)header + bswap32(header->off_dt_strings);

  for (;;) {
    uint32_t token = bswap32(*(uint32_t*)ptr);
    ptr += 4;

    switch (token) {
      case FDT_BEGIN_NODE:
        char* name = (char*)ptr;
        kprintf("%s\n", name);
        ptr += strlen(name);
        ptr = align_up(ptr, 4);
        break;
      case FDT_END_NODE:
        break;
      case FDT_PROP:
        uint32_t len = bswap32(*(uint32_t*)ptr);
        uint32_t name_offset = bswap32(*(uint32_t*)(ptr + 4));
        ptr += 8 + len;
        ptr = align_up(ptr, 4);
        kprintf("- %s\n", strings + name_offset);
        break;
      case FDT_NOP:
        break;
      case FDT_END:
        return;
      default:
        kprintf("fdt_dump: unknown token\n");
        return;
    }
  }
}
