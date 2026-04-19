#ifndef CPU_IO_H
#define CPU_IO_H

#include <stdint.h>

// Assembly OUTB instruction
inline void outb(uint16_t port, uint8_t val)
{
        __asm__ volatile (
                "outb %0, %1"
                :
                : "a"(val), "Nd"(port)
        );
}

// Assembly INB instruction
inline uint8_t inb(uint16_t port)
{
        uint8_t val;
        __asm__ volatile (
                "inb %1, %0"
                : "=a"(val)
                : "Nd"(port)
        );
        return val;
}

#endif /* CPU_IO_H */