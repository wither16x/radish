#ifndef MEM_PMM_H
#define MEM_PMM_H

#include <limine.h>
#include <stdint.h>

// Initialize the PMM using the memory map
void pmm_init(struct limine_memmap_response *memmap);
// Allocate a free physical frame
uintptr_t pmm_allocate_frame();
// Free an allocated physical frame
void pmm_free_frame(uintptr_t addr);

#endif /* MEM_PMM_H */