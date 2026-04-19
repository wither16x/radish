#ifndef KLIB_PRINT_H
#define KLIB_PRINT_H

#include <stdarg.h>

// RadishOS kernel string formatting rules:
// * %c         : character
// * %s         : string
// * %b         : unsigned binary integer
// * %o         : unsigned octal integer
// * %d         : unsigned decimal integer
// * %x         : unsigned hexadecimal integer
// * %%         : % character

// Both functions below format a string and print them
void vprintf(const char *fmt, va_list args);
void printf(const char *fmt, ...);

#endif /* KLIB_PRINT_H */