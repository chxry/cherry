#include "fdt.h"
#include "printf.h"
#include "lib/mem.h"

fdt_header_t* fdt_parse_header(size_t fdt_addr) {
  fdt_header_t* header = (fdt_header_t*)fdt_addr;
  if (bswap32(header->magic) != FDT_MAGIC)
    return 0;
  return header;
}

#define FDT_TRAVERSE(header, on_node, on_prop, on_end) \
  uint8_t* ptr = (uint8_t*)header + bswap32(header->off_dt_struct); \
  char* strings = (char*)header + bswap32(header->off_dt_strings); \
  for (;;) { \
    uint32_t token = bswap32(*(uint32_t*)ptr); \
    ptr += 4; \
    switch (token) { \
      case FDT_BEGIN_NODE: { \
        char* name = (char*)ptr; \
        on_node \
        ptr += strlen(name); \
        ptr = (uint8_t*)align_up((size_t)ptr, 4); \
        break; \
      } \
      case FDT_END_NODE: \
        break; \
      case FDT_PROP: {\
        uint32_t len = bswap32(*(uint32_t*)ptr); \
        uint32_t name_offset = bswap32(*(uint32_t*)(ptr + 4)); \
        ptr += 8; \
        char* name = strings + name_offset; \
        on_prop \
        ptr += len; \
        ptr = (uint8_t*)align_up((size_t)ptr, 4); \
        break; \
      } \
      case FDT_NOP: \
        break; \
      case FDT_END: \
        on_end \
      default: \
        kprintf("%s: unknown token %x\n", __func__, token); \
        on_end \
    } \
  } \


uint8_t* fdt_find_by_prop(const fdt_header_t* header, const char* prop_name, const uint8_t* prop_val, uint32_t prop_len) {
  uint8_t* last_node = 0;
  FDT_TRAVERSE(
    header,
    last_node = ptr;,
    {
      if (len == prop_len && strcmp(prop_name, name)) {
        for (uint32_t i = 0; i < len; i++) {
          if (prop_val[i] != ptr[i]) {
            goto todo;
          }
        }
        return last_node;
      }
      todo:
    },
    return 0;
  );
}

void fdt_dump(const fdt_header_t* header) {
  FDT_TRAVERSE(
    header,
    kprintf("%s\n", name);,
    kprintf("- %s\n", name);,
    return;
  );
}
