#ifndef DRIVERS_SERIAL_H
#define DRIVERS_SERIAL_H

// Initialize the serial port
int serial_init();
// Send a single character to the serial port
void serial_send_char(char ch);
// Send a string to the serial port
void serial_send_str(const char *s);

#endif /* DRIVERS_SERIAL_H */