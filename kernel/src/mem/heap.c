#include <mem/heap.h>
#include <panic.h>
#include <stddef.h>
#include <stdint.h>

extern uint64_t __heap_start[];
extern uint64_t __heap_end[];

static constexpr uint64_t HEAP_SIZE     = 0x20000000;
static constexpr uint64_t BLOCK_SIZE    = 64;
static constexpr uint64_t MAX_BLOCKS    = HEAP_SIZE / BLOCK_SIZE;

static uint64_t bitmap[MAX_BLOCKS];
static uint64_t last_block = 0;

static void block_set_allocated(uint64_t i)
{
        if (i < MAX_BLOCKS) {
                int idx = i / (sizeof(uint64_t) * 8);
                int bit = i % (sizeof(uint64_t) * 8);
                bitmap[idx] |= 1ull << bit;
        }
}

static void block_set_free(uint64_t i)
{
        if (i < MAX_BLOCKS) {
                int idx = i / (sizeof(uint64_t) * 8);
                int bit = i % (sizeof(uint64_t) * 8);
                bitmap[idx] &= ~(1ull << bit);
        }
}

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
        for (uint64_t i = 0; i < MAX_BLOCKS; i++)
                block_set_free(i);
}

void *heap_alloc(size_t n)
{
        size_t blocks = (n + BLOCK_SIZE - 1) / BLOCK_SIZE;

        for (size_t start = 0; start +  blocks <= MAX_BLOCKS; ++start) {
                bool not_found = true;

                for (size_t i = 0; i < blocks; i++) {
                        if (get_block(start + i)) {
                                not_found = false;
                                break;
                        }
                }

                if (!not_found)
                        continue;

                for (size_t i = 0; i < blocks; i++)
                        block_set_allocated(start + i);

                return (void *)(start + last_block * BLOCK_SIZE);
        }

        panic(
                "out of heap memory\r\n"
                "last allocated block: %d",
                last_block
        );

        return NULL;
}

void heap_free(void *p)
{
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

        if (first_block < last_block)
                last_block = first_block;
}