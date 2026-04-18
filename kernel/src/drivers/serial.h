#ifndef DRIVERS_SERIAL_H
#define DRIVERS_SERIAL_H

int serial_init();
void serial_send_str(const char *s);

#endif /* DRIVERS_SERIAL_H */