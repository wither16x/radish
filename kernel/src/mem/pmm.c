#include <klib/logging.h>
#include <limine.h>
#include <mem/pmm.h>
#include <panic.h>
#include <stdint.h>

// The PMM only handles the first 2GB of RAM.
// To be changed later, but this is enough for old machines.
#define TWO_GIGABYTES                   0x80000000

static constexpr int FRAME_SIZE                = 4096;          // 4 KiB
static constexpr uint64_t MAX_FRAMES           = TWO_GIGABYTES / FRAME_SIZE; 

static uint64_t bitmap[MAX_FRAMES];
static uint64_t last_frame = 0;         // last allocated frame

// Mark a frame as allocated in the bitmap
static void frame_set_allocated(uint64_t i)
{
        if (i < MAX_FRAMES) {
                int idx = i / (sizeof(uint64_t) * 8);
                int bit = i % (sizeof(uint64_t) * 8);
                bitmap[idx] |= 1ull << bit;
        }
}

// Mark a frame as free in the bitmap
static void frame_set_free(uint64_t i)
{
        if (i < MAX_FRAMES) {
                int idx = i / (sizeof(uint64_t) * 8);
                int bit = i % (sizeof(uint64_t) * 8);
                bitmap[idx] &= ~(1ull << bit);
        }
}

// Get a bit from the bitmap
static bool get_frame(uint64_t i)
{
        if (i < MAX_FRAMES) {
                uint64_t idx = i / (sizeof(uint64_t) * 8);
                uint64_t bit = i % (sizeof(uint64_t) * 8);
                return (bitmap[idx] >> bit) & 1ull;
        } else {
                return false;
        }
}

void pmm_init(struct limine_memmap_response *memmap)
{
        // Allocate everything
        for (uint64_t i = 0; i < MAX_FRAMES; i++)
                frame_set_allocated(i);

        // Then free all usable frames
        for (uint64_t i = 0; i < memmap->entry_count; i++) {
                if (memmap->entries[i]->type == LIMINE_MEMMAP_USABLE) {
                        struct limine_memmap_entry *e = memmap->entries[i];
                        uint64_t start = e->base / FRAME_SIZE;
                        uint64_t end = (e->base + e->length + FRAME_SIZE - 1) / FRAME_SIZE;

                        for (uint64_t f = start; f < end; f++) {
                                if (f < MAX_FRAMES)
                                        frame_set_free(f);
                        }
                }
        }
}

uintptr_t pmm_allocate_frame()
{
        // Look for a free frame
        while (last_frame < MAX_FRAMES && get_frame(last_frame))
                last_frame++;

        if (last_frame >= MAX_FRAMES) {
                panic(
                        "out of physical memory\r\n"
                        "Last allocated frame: %d",
                        last_frame
                );
        }

        // Allocate the frame
        frame_set_allocated(last_frame);
        return last_frame * FRAME_SIZE;
}

void pmm_free_frame(uintptr_t addr)
{
        uint64_t idx = addr / FRAME_SIZE;
        
        if (get_frame(idx)) {
                frame_set_free(idx);
                last_frame = idx;
        } else {
                panic("double free on frame %d", idx);
        }
}