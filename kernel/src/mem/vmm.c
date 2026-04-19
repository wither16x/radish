#include <klib/memory.h>
#include <limine.h>
#include <mem/pmm.h>
#include <mem/vmm.h>
#include <stdint.h>

extern uint64_t __kernel_start[];
extern uint64_t __kernel_end[];

constexpr uint64_t PAGE_MASK = 0xffffffffff000;

static uint64_t *pml4t;
static struct limine_hhdm_response *hhdm_response;
static struct limine_executable_address_response *exec_response;
static struct limine_memmap_response *memmap_response;

static void map_kernel()
{
        uint64_t kernel_start     = (uint64_t)*(&__kernel_start);
        uint64_t kernel_end       = (uint64_t)*(&__kernel_end);
        uint64_t kernel_size      = page_div_up(kernel_end - kernel_start);
        uint64_t phys             = exec_response->physical_base;
        uint64_t virt             = exec_response->virtual_base;

        for (uint64_t i = 0; i < kernel_size; i++) {
                vmm_map_page(virt, phys, PAGE_READ_WRITE);
                phys += PAGE_SIZE;
                virt += PAGE_SIZE;
        }
}

static void map_hhdm()
{
        for (uint64_t i = 0; i < memmap_response->entry_count; i++) {
                int t = memmap_response->entries[i]->type;
                if (t == LIMINE_MEMMAP_ACPI_NVS
                    || t == LIMINE_MEMMAP_ACPI_RECLAIMABLE
                    || t == LIMINE_MEMMAP_BOOTLOADER_RECLAIMABLE
                    || t == LIMINE_MEMMAP_FRAMEBUFFER
                    || t == LIMINE_MEMMAP_EXECUTABLE_AND_MODULES
                    || t == LIMINE_MEMMAP_USABLE
                ) {
                        uint64_t section_size = page_div_up(memmap_response->entries[i]->length);
                        uint64_t phys = memmap_response->entries[i]->base;
                        uint64_t virt = memmap_response->entries[i]->base + hhdm_response->offset;

                        for (uint64_t i = 0; i < section_size; i++) {
                                vmm_map_page(virt, phys, PAGE_READ_WRITE);
                                phys += PAGE_SIZE;
                                virt += PAGE_SIZE;
                        }
                }
        }
}

static void init_pml4t()
{
        pml4t = (uint64_t *)(PAGE_SIZE * pmm_allocate_frame() + hhdm_response->offset);
        memset(pml4t, 0, PAGE_SIZE);
        map_kernel();
        map_hhdm();
}

void vmm_init(
        struct limine_hhdm_response *hhdm,
        struct limine_executable_address_response *exec,
        struct limine_memmap_response *memmap
)
{
        hhdm_response = hhdm;
        exec_response = exec;
        memmap_response = memmap;

        init_pml4t();

        uint64_t cr3 = (uint64_t)pml4t - hhdm_response->offset;
        __asm__ volatile (
                "movq %0, %%cr3"
                :
                : "r"(cr3)
        );
}

void vmm_map_page(uint64_t virt, uint64_t phys, enum page_flag flags)
{
        uint64_t hhdm = hhdm_response->offset;

        uint64_t pml4t_idx        = (virt >> 39) & 0x1ff;
        uint64_t pdpt_idx         = (virt >> 30) & 0x1ff;
        uint64_t pdt_idx          = (virt >> 21) & 0x1ff;
        uint64_t pt_idx           = (virt >> 12) & 0x1ff;
        
        if (!(pml4t[pml4t_idx] & 1)) {
                pml4t[pml4t_idx] = pmm_allocate_frame() + PAGE_READ_WRITE;
                memset(
                        (uint64_t*)((pml4t[pml4t_idx] & PAGE_MASK) + hhdm),
                        0,
                        PAGE_SIZE
                );
        }

        uint64_t *pdpt = (uint64_t*)((pml4t[pml4t_idx] & PAGE_MASK) + hhdm);
        if (!(pdpt[pdpt_idx] & 1)) {
                pdpt[pdpt_idx] = pmm_allocate_frame() + PAGE_READ_WRITE;
                memset(
                        (uint64_t*)((pdpt[pdpt_idx] & PAGE_MASK) + hhdm),
                        0,
                        PAGE_SIZE
                );
        }

        uint64_t *pdt = (uint64_t*)((pdpt[pdpt_idx] & PAGE_MASK) + hhdm);
        if (!(pdt[pdt_idx] & 1)) {
                pdt[pdt_idx] = pmm_allocate_frame() + PAGE_READ_WRITE;
                memset(
                        (uint64_t*)((pdt[pdt_idx] & PAGE_MASK) + hhdm),
                        0,
                        PAGE_SIZE
                );
        }

        uint64_t *pt = (uint64_t*)((pdt[pdt_idx] & PAGE_MASK) + hhdm);
        if (!(pt[pt_idx] & 1))
                pt[pt_idx] = phys | flags;
}