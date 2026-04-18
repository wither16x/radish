#ifndef KLIB_PRINT_H
#define KLIB_PRINT_H

#include <stdarg.h>

void vprintf(const char *fmt, va_list args);
void printf(const char *fmt, ...);

#endif /* KLIB_PRINT_H */