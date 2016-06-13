

#ifndef SERIAL_WIN32
#define SERIAL_WIN32





int serial_begin(char *sComPort, unsigned int baudrate);

unsigned int serial_read(unsigned char *buf, unsigned int len);

unsigned int serial_write(unsigned char *buf, unsigned int len);

void serial_end(void);





#endif