#include <klib/logging.h>
#include <klib/print.h>
#include <stdarg.h>

static const char *log_level_str_table[] = {
        "debug",
        "info",
        "warn",
        "error",
        "fatal"
};

void log(enum log_level level, const char *fmt, ...)
{
        printf("%s: ", log_level_str_table[level]);
        va_list args;
        va_start(args, fmt);
        vprintf(fmt, args);
        va_end(args);
        printf("\r\n");
}