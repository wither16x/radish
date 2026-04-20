#include <klib/string.h>
#include <stddef.h>

// Check if a character is a delimitator.
// Used by `strtok()`.
static int is_delim(char c, char *delim)
{
        while (*delim) {
                if (c == *delim)
                        return 1;
                delim++;
        }

        return 0;
}

int strcmp(const char *s1, const char *s2)
{
        int i = 0;
        while (s1[i] == s2[i]) {
                if (s1[i] == '\0' || s2[i] == '\0')
                        break;
                i++;
        }

        if (s1[i] == '\0' && s2[i] == '\0')
                return 0;
        else
                return s1[i] - s2[i];
}

char *strtok(char *s, char *delim)
{
        static char *bak;

        if (!s)
                s = bak;
        if (!s)
                return NULL;

        while (true) {
                if (is_delim(*s, delim)) {
                        s++;
                        continue;
                }

                if (*s == '\0')
                        return NULL;

                break;
        }

        char *ret = s;
        while (true) {
                if (*s == '\0') {
                        bak = s;
                        return ret;
                }

                if (is_delim(*s, delim)) {
                        *s = '\0';
                        bak = s + 1;
                        return ret;
                }

                s++;
        }
}