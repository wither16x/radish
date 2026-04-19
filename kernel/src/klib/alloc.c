#include <klib/alloc.h>
#include <mem/heap.h>

void *malloc(size_t n)
{
        return heap_alloc(n);
}

void free(void *p)
{
        heap_free(p);
}