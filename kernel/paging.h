#pragma once
#include <stddef.h>

#define PAGE_SIZE 4096
#define PAGETABLE_ENTRIES 512

#define SATP_SV39 (8ULL << 60)
#define PTE_VALID (1 << 0)
#define PTE_READ (1 << 1)
#define PTE_WRITE (1 << 2)
#define PTE_EXECUTE (1 << 3)
#define PTE_USER (1 << 4)

typedef size_t pte_t;
typedef pte_t* pagetable_t;

void paging_init(void);
void paging_map_page(pagetable_t pagetable, size_t va, size_t pa, size_t flags);
void paging_identity_map_range(pagetable_t pagetable, size_t start, size_t end, size_t flags);
void paging_print_pagetable(pagetable_t pagetable, int level, size_t va_base);
