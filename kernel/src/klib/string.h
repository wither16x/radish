#ifndef KLIB_STRING_H
#define KLIB_STRING_H

// Compare two strings
// * < 0        : s1 < s2
// * = 0        : s1 = s2
// * > 0        : s1 > s2
int strcmp(const char *s1, const char *s2);
// Break a string into several tokens.
// Use `delim` as a separator.
char *strtok(char *s, char *delim);

#endif /* KLIB_STRING_H */