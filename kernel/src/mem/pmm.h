#ifndef MEM_PMM_H
#define MEM_PMM_H

#include <limine.h>
#include <stdint.h>

void pmm_init(struct limine_memmap_response *memmap);
uintptr_t pmm_allocate_frame();
void pmm_free_frame(uintptr_t addr);

#endif /* MEM_PMM_H */