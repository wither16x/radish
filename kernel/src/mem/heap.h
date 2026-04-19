#ifndef MEM_HEAP_H
#define MEM_HEAP_H

#include <stddef.h>

// Initialize the heap
void heap_init();
// Allocate memory blocks and return a pointer
void *heap_alloc(size_t n);
// Free a pointer
void heap_free(void *p);

#endif /* MEM_HEAP_H */