#include <stddef.h>
#include <stdint.h>
#include "printf.h"
#include "paging.h"
#include "fdt.h"
#include "lib/mem.h"

void abort(void);

void kmain(size_t hart_id, size_t fdt_addr) {
  kprintf("cherry %d\n", 2);

  fdt_header_t* fdt_header = fdt_parse_header(fdt_addr);
  if (!fdt_header) {
    kprintf("invalid fdt header");
    abort();
  }
  size_t fdt_size = bswap32(fdt_header->totalsize);

  paging_init();
  paging_identity_map_range(root_pagetable, fdt_addr, fdt_addr + fdt_size, PTE_READ);

  // int* addr = kmalloc(512);
  // addr[3] = 123;
  // kprintf("%p %d\n", addr, addr[3]);

  // paging_print_pagetable(root_pagetable, 2, 0);
  // fdt_dump(fdt_header);
  uint8_t* test = fdt_find_by_prop(fdt_header, "compatible", (uint8_t*)"ns16550a", 9);
  kprintf("test: %p\n", test);
 
  abort();
}

void trap_handler(size_t scause, size_t stval, size_t sepc) {
  kprintf("trap: scause=%x, stval=%x, sepc=%x\n", scause, stval, sepc);
  if (scause >> 63) {
    kprintf("interrupt ...\n");
  } else {
    switch ((scause << 1) >> 1) {
      case 0:
        kprintf("instruction address misaligned\n");
        break;
      case 1:
        kprintf("instruction access fault\n");
        break;
      case 2:
        kprintf("illegal instruction\n");
        break;
      case 3:
        kprintf("breakpoint\n");
        break;
      case 4:
        kprintf("load access misaligned\n");
        break;
      case 5:
        kprintf("load access fault\n");
        break;
      case 6:
        kprintf("store/amo address misaligned\n");
        break;
      case 7:
        kprintf("store/amo access fault\n");
        break;
      case 12:
        kprintf("instruction page fault\n");
        break;
      case 13:
        kprintf("load page fault\n");
        break;
      case 15:
        kprintf("store/amo page fault\n");
        break;
      case 18:
        kprintf("software check\n");
        break;
      case 19:
        kprintf("hardware error\n");
        break;
      default:
        kprintf("unknown exception\n");
        break;
        
    }
  }
  abort();
}

void abort(void) {
  for (;;) {
    asm volatile("wfi");
  }
}
