#include <klib/print.h>
#include <panic.h>
#include <stdarg.h>

void panic(const char *fmt, ...)
{
        printf("-------------------- Kernel Panic --------------------\r\n");
        printf("Because: ");
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\r\n");

        while (true)
                __asm__ ("hlt");
}