#include <drivers/serial.h>
#include <klib/print.h>
#include <stdarg.h>
#include <stddef.h>

static void putchar(char ch)
{
        serial_send_char(ch);
}

static void print_str(const char *s)
{
        while (*s)
                putchar(*s++);
}

static void print_int(size_t n, int base)
{
        char buf[32];
        bool neg = false;
        char *p = buf;

        if (n == 0) {
                *p++ = '0';
                *p = '\0';
                return;
        }

        while (n) {
                int digit = n % base;
                *p++ = (digit < 10) ? (digit + '0') : (digit - 10 + 'a');
                n /= base;
        }

        if (neg)
                *p++ = '-';

        *p = '\0';

        char *start = buf;
        char *end = p - 1;
        while (start < end) {
                char tmp = *start;
                *start++ = *end;
                *end-- = tmp;
        }

        print_str(buf);
}

static void vprintf(const char *fmt, va_list args)
{
        while (*fmt) {
                if (*fmt == '%') {
                        fmt++;

                        switch (*fmt) {
                        case '%':
                                putchar('%');
                                break;

                        case 'c': {
                                char ch = (char)va_arg(args, int);
                                putchar(ch);
                                break;
                        }

                        case 's': {
                                const char *s = va_arg(args, const char *);
                                print_str(s);
                                break;
                        }

                        case 'b': {
                                size_t n = va_arg(args, size_t);
                                print_int(n, 2);
                                break;
                        }

                        case 'o': {
                                size_t n = va_arg(args, size_t);
                                print_int(n, 8);
                                break;
                        }

                        case 'd': {
                                size_t n = va_arg(args, size_t);
                                print_int(n, 10);
                                break;
                        }

                        case 'x': {
                                size_t n = va_arg(args, size_t);
                                print_int(n, 16);
                                break;
                        }

                        default:
                                break;
                        }
                } else {
                        putchar(*fmt);
                }

                fmt++;
        }
}

void printf(const char *fmt, ...)
{
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
}