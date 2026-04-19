#include <mem/heap.h>
#include <panic.h>
#include <stddef.h>
#include <stdint.h>

extern uint64_t __heap_start[];
extern uint64_t __heap_end[];

static constexpr uint64_t HEAP_SIZE     = 0x20000000;           // 512 MiB
static constexpr uint64_t BLOCK_SIZE    = 64;                   // 64 bytes allocated for 1 byte
static constexpr uint64_t MAX_BLOCKS    = HEAP_SIZE / BLOCK_SIZE;

static uint64_t bitmap[(MAX_BLOCKS + sizeof(uint64_t) * 8 - 1) / (sizeof(uint64_t) * 8)];

// Mark a block as allocated in the bitmap
static void block_set_allocated(uint64_t i)
{
        if (i < MAX_BLOCKS) {
                int idx = i / (sizeof(uint64_t) * 8);
                int bit = i % (sizeof(uint64_t) * 8);
                bitmap[idx] |= 1ull << bit;
        }
}

// Mark a block as free in the bitmap
static void block_set_free(uint64_t i)
{
        if (i < MAX_BLOCKS) {
                int idx = i / (sizeof(uint64_t) * 8);
                int bit = i % (sizeof(uint64_t) * 8);
                bitmap[idx] &= ~(1ull << bit);
        }
}

// Get a bit from the bitmap
static bool get_block(uint64_t i)
{
        if (i < MAX_BLOCKS) {
                uint64_t idx = i / (sizeof(uint64_t) * 8);
                uint64_t bit = i % (sizeof(uint64_t) * 8);
                return (bitmap[idx] >> bit) & 1ull;
        } else {
                return false;
        }
}

void heap_init()
{
        // Free all blocks
        for (uint64_t i = 0; i < MAX_BLOCKS; i++)
                block_set_free(i);
}

void *heap_alloc(size_t n)
{
        // nothing to allocate
        if (n == 0)
                return NULL;

        // space for the header
        size_t total = n + sizeof(size_t);
        // number of blocks needed
        size_t blocks = (total + BLOCK_SIZE - 1) / BLOCK_SIZE;

        for (size_t start = 0; start +  blocks <= MAX_BLOCKS; ++start) {
                bool found = true;

                // Look for a free block
                for (size_t i = 0; i < blocks; i++) {
                        if (get_block(start + i)) {
                                found = false;
                                break;
                        }
                }

                if (!found)
                        continue;

                for (size_t i = 0; i < blocks; i++)
                        block_set_allocated(start + i);

                uintptr_t addr = (uintptr_t)__heap_start + start * BLOCK_SIZE;
                size_t *hdr = (size_t *)addr;
                *hdr = blocks;
                return (void*)(hdr + 1);
        }

        panic("out of heap memory");

        // To avoid compiler warnings, should never be reached
        return NULL;
}

void heap_free(void *p)
{
        // As you can see, this function only clears a bit the bitmap, so
        // you can do use-after-free (`heap_free(p); *p = 42;` would work).
        // However, do not do that because the cleared blocks can be reused
        // by `heap_alloc()`.

        if (!p)
                return;

        size_t *base = (size_t *)p - 1;
        size_t blocks = *base;
        size_t first_block = ((size_t)base - (size_t)__heap_start) / BLOCK_SIZE;

        for (size_t i = 0; i < blocks; ++i) {
                size_t idx = first_block + i;

                if (!get_block(idx))
                        panic("double free on block %d", idx);

                block_set_free(idx);
        }
}