#ifndef PANIC_H
#define PANIC_H

// Display a formatted error message, cancel the interrupts
// and stops the kernel forever.
// To be used only if there is no alternative.
void panic(const char *fmt, ...);

#endif /* PANIC_H */