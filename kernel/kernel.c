#include <stddef.h>
#include "paging.h"
#include "printf.h"

void kmain(size_t hart_id, size_t fdt_addr) {
  kprintf("cherry %d\n", 2);
  kprintf("hart_id = %d\n", hart_id);
  kprintf("fdt_addr = %p\n", fdt_addr);

  paging_init();

  for (;;);
}

void trap_handler(size_t scause, size_t stval, size_t sepc) {
  kprintf("trap: scause=%x, stval=%x, sepc=%x\n", scause, stval, sepc);
  for (;;);
}
