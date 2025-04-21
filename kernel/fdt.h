#pragma once
#include <stddef.h>
#include <stdint.h>

#define FDT_MAGIC 0xd00dfeed
#define FDT_BEGIN_NODE 0x1
#define FDT_END_NODE 0x2
#define FDT_PROP 0x3
#define FDT_NOP 0x4
#define FDT_END 0x9

typedef struct {
  uint32_t magic;
  uint32_t totalsize;
  uint32_t off_dt_struct;
  uint32_t off_dt_strings;
  uint32_t off_mem_rsvmap;
  uint32_t version;
  uint32_t last_comp_version;
  uint32_t boot_cpuid_phys;
  uint32_t size_dt_strings;
  uint32_t size_dt_struct;
} fdt_header_t;

fdt_header_t* fdt_parse_header(size_t fdt_addr);
uint8_t* fdt_find_by_prop(const fdt_header_t* header, const char* prop_name, const uint8_t* prop_val, uint32_t prop_len);
void fdt_dump(const fdt_header_t* header);
