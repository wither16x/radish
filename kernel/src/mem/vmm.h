#ifndef MEM_VMM_H
#define MEM_VMM_H

#include <limine.h>
#include <stdint.h>

constexpr int PAGE_SIZE         = 4096;         // 4 KiB

// Flags that a page can have.
// Note that they are not all listed here, these are just
// the one I use.
enum page_flag {
        PAGE_PRESENT            = 0x01,
        PAGE_READ_WRITE         = 0x03,
        PAGE_NO_EXEC            = 1ull << 63
};

// Initialize the VMM
void vmm_init(
        struct limine_hhdm_response *hhdm,
        struct limine_executable_address_response *exec,
        struct limine_memmap_response *memmap
);
// Map a page with custom flags
void vmm_map_page(uint64_t virt, uint64_t phys, enum page_flag flags);

// Helpers
// ================================================================================
static inline uint64_t page_align_down(uint64_t addr)
{
        return addr / PAGE_SIZE * PAGE_SIZE;
}

static inline uint64_t page_align_up(uint64_t addr)
{
        return (addr + PAGE_SIZE - 1) / PAGE_SIZE * PAGE_SIZE;
}

static inline uint64_t page_div_up(uint64_t addr)
{
        return (addr + PAGE_SIZE - 1) / PAGE_SIZE;
}
// ================================================================================

#endif /* MEM_VMM_H */