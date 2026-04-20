#include <klib/alloc.h>
#include <klib/memory.h>
#include <mem/heap.h>

void *malloc(size_t n)
{
        return heap_alloc(n);
}

void *realloc(void *p, size_t len)
{
        void *real = malloc(len);
        memset(real, 0, len);

        if (real)
                memcpy(real, p, len);

        free(p);
        return real;
}

void free(void *p)
{
        heap_free(p);
}