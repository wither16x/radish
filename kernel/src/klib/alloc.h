#ifndef KLIB_ALLOC_H
#define KLIB_ALLOC_H

#include <stddef.h>

// Dynamically allocate `n` bytes for a pointer
void *malloc(size_t n);
// Reallocate a dynamically allocated pointer
void *realloc(void *p, size_t len);
// Free a dynamically allocated pointer
void free(void *p);

#endif /* KLIB_ALLOC_H */