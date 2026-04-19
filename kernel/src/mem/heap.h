#ifndef MEM_HEAP_H
#define MEM_HEAP_H

#include <stddef.h>

void heap_init();
void *heap_alloc(size_t n);
void heap_free(void *p);

#endif /* MEM_HEAP_H */