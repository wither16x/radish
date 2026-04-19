#ifndef KLIB_MEMORY_H
#define KLIB_MEMORY_H

#include <stddef.h>

// DO NOT change the name of the functions below, as the compiler reserves the
// right to call them.

// Copy `n` bytes from `src` to `dest`
void *memcpy(void *restrict dest, const void *restrict src, size_t n);
// Set `n` bytes of `s` to `c`
void *memset(void *s, int c, size_t n);
// Copy `n` bytes from `src` to `dest` and avoid overlaps
void *memmove(void *dest, const void *src, size_t n);
// Compare `n` bytes from `s1` and `s2`
// Return values:
// * > 0 if `s1` > `s2`
// * = 0 if `s1` = `s2`
// * < 0 if `s1` < `s2`
int memcmp(const void *s1, const void *s2, size_t n);

#endif /* KLIB_MEMORY_H */