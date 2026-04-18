#include <cpu/io.h>
#include <drivers/serial.h>
#include <stdint.h>

constexpr uint16_t COM1 = 0x3f8;

int serial_init()
{
        outb(COM1 + 1, 0x00);
        outb(COM1 + 3, 0x80);
        outb(COM1, 0x03);
        outb(COM1 + 1, 0x00);
        outb(COM1 + 3, 0x03);
        outb(COM1 + 2, 0xc7);
        outb(COM1 + 4, 0x0b);
        outb(COM1 + 4, 0x1e);

        outb(COM1, 0xae);
        if (inb(COM1) != 0xae)
                return -1;

        outb(COM1 + 4, 0x0f);
        return 0;
}

void serial_send_char(char ch)
{
        while ((inb(COM1 + 5) & 0x20) == 0);
        outb(COM1, ch);
}

void serial_send_str(const char *s)
{
        while (*s)
                serial_send_char(*s++);
}