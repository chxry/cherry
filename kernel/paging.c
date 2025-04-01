#include "paging.h"
#include "printf.h"
#include "lib/mem.h"

extern char _kernel_start, _heap_start;

pagetable_t root_pagetable;

void* alloc_page(void) {
  static void* next = &_heap_start;
  void* addr = next;
  memset(addr, 0, PAGE_SIZE);
  next += PAGE_SIZE;
  return addr;
}

void paging_init(void) {
  root_pagetable = alloc_page();

  paging_identity_map_range(root_pagetable, (size_t)&_kernel_start, (size_t)&_heap_start, PTE_READ | PTE_WRITE | PTE_EXECUTE);
  // find a better solution for mapping the heap
  paging_identity_map_range(root_pagetable, (size_t)&_heap_start, (size_t)&_heap_start + PAGE_SIZE * 1024, PTE_READ | PTE_WRITE);
  // paging_print_pagetable(root_pagetable, 2, 0);

  size_t satp_value = SATP_SV39 | ((size_t)root_pagetable >> 12);
  asm volatile("csrw satp, %0" :: "r"(satp_value));
  asm volatile("sfence.vma");
  kprintf("paged!\n");
}

void paging_map_page(pagetable_t pagetable, size_t va, size_t pa, size_t flags) {
  for (int level = 2; level > 0; level--) {
    size_t index = (va >> (12 + level * 9)) & 0x1ff;

    if (!(pagetable[index] & PTE_VALID)) {
      pte_t* new_page = alloc_page();
      pagetable[index] = (((size_t)new_page >> 12) << 10) | PTE_VALID;
    }
    pagetable = (pagetable_t)((pagetable[index] >> 10) << 12);
  }

  size_t index = (va >> 12) & 0x1ff;
  pagetable[index] = ((pa >> 12) << 10) | flags | PTE_VALID;
}

void paging_identity_map_range(pagetable_t pagetable, size_t start, size_t end, size_t flags) {
  for (size_t addr = start; addr < end; addr += PAGE_SIZE)
    paging_map_page(pagetable, addr, addr, flags);
}

void paging_print_pagetable(pagetable_t pagetable, int level, size_t va_base) {
  size_t region_size = 1 << (12 + level * 9);
  
  for (int i = 0; i < PAGETABLE_ENTRIES; i++) {
    pte_t entry = pagetable[i];
    if (!(entry & PTE_VALID))
      continue;

    uint64_t va = va_base + i * region_size;
    uint64_t pa = (entry >> 10) << 12;

    if (entry & (PTE_READ | PTE_WRITE | PTE_EXECUTE)) {
        kprintf("virt %x -> phys %x (flags %07b)\n", va, pa, entry & 0xff);
    } else {
        paging_print_pagetable((pagetable_t)pa, level - 1, va);
    }
  }
}
