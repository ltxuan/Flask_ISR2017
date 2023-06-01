#include <stdio.h>
#include <string.h>
#include <stdint.h>
#include <stdlib.h>
// Linux headers
#include <fcntl.h> // Contains file controls like O_RDWR
#include <errno.h> // Error integer and strerror() function
#include <termios.h> // Contains POSIX terminal control definitions
#include <unistd.h> // write(), read(), close()

struct buf_uart_rx
{
    unsigned char read_buf[20];
    unsigned char avail;
};
int fd1;
int init_uart();
void transfer(int fd, unsigned char byte);
void transfer_9_byte(int fd);
void transfer_n_byte(int fd, unsigned char *tx_buf);
void transfer_2_byte(int fd, unsigned char byte, unsigned char byte2);
void transfer_phone_byte(int fd, unsigned char *tx_buf);