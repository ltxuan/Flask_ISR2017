#include "uart.h"

int init_uart(){
  // Open the serial port. Change device path as needed (currently set to an standard FTDI USB-UART cable type device)
  int serial_port = open("/dev/ttyUSB0", O_RDWR);

  // Create new termios struc, we call iact 'tty' for convention
  struct termios tty;

  // Read in existing settings, and handle any error
  if(tcgetattr(serial_port, &tty) != 0) {
      printf("Error %i from tcgetattr: %s\n", errno, strerror(errno));
      return 1;
  }

  tty.c_cflag &= ~PARENB; // Clear parity bit, disabling parity (most common)
  tty.c_cflag &= ~CSTOPB; // Clear stop field, only one stop bit used in communication (most common)
  tty.c_cflag &= ~CSIZE; // Clear all bits that set the data size 
  tty.c_cflag |= CS8; // 8 bits per byte (most common)
  tty.c_cflag &= ~CRTSCTS; // Disable RTS/CTS hardware flow control (most common)
  tty.c_cflag |= CREAD | CLOCAL; // Turn on READ & ignore ctrl lines (CLOCAL = 1)

  tty.c_lflag &= ~ICANON;
  tty.c_lflag &= ~ECHO; // Disable echo
  tty.c_lflag &= ~ECHOE; // Disable erasure
  tty.c_lflag &= ~ECHONL; // Disable new-line echo
  tty.c_lflag &= ~ISIG; // Disable interpretation of INTR, QUIT and SUSP
  tty.c_iflag &= ~(IXON | IXOFF | IXANY); // Turn off s/w flow ctrl
  tty.c_iflag &= ~(IGNBRK|BRKINT|PARMRK|ISTRIP|INLCR|IGNCR|ICRNL); // Disable any special handling of received bytes

  tty.c_oflag &= ~OPOST; // Prevent special interpretation of output bytes (e.g. newline chars)
  tty.c_oflag &= ~ONLCR; // Prevent conversion of newline to carriage return/line feed
  // tty.c_oflag &= ~OXTABS; // Prevent conversion of tabs to spaces (NOT PRESENT ON LINUX)
  // tty.c_oflag &= ~ONOEOT; // Prevent removal of C-d chars (0x004) in output (NOT PRESENT ON LINUX)

  tty.c_cc[VTIME] = 0;    
  tty.c_cc[VMIN] = 10;      // Wait until read enough 9 byte

  // Set in/out baud rate to be 9600
  cfsetispeed(&tty, B230400);
  cfsetospeed(&tty, B230400);

  // Save tty settings, also checking for error
  if (tcsetattr(serial_port, TCSANOW, &tty) != 0) {
      printf("Error %i from tcsetattr: %s\n", errno, strerror(errno));
      return -1;
  }
  return serial_port;
}
void transfer(int fd, unsigned char byte)
{
    int ret;
    uint8_t tx[10] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF
	};
    tx[0] = byte;
    ret = write(fd, tx, 10);
    if (ret < 0){
        printf("send byte error");
        return;
    }
    else printf("send %.2X to stm32 success\n", byte);
}
void transfer_9_byte(int fd)
{
    int ret;
    uint8_t tx[9] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF
	};
    ret = write(fd, tx, 9);
    if (ret < 0){
        printf("send byte error");
        return;
    }
    else printf("send 9 byte to stm32 success\n");
}
void transfer_2_byte(int fd, unsigned char byte, unsigned char byte2)
{
    int ret;
    uint8_t tx[10] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF, 0xFF
	};
    tx[0] = byte;
    tx[1] = byte2;
    ret = write(fd, tx, 10);
    if (ret < 0){
        printf("send byte error");
        return;
    }
    else printf("send %.2X to stm32 success\n", byte);
}
void transfer_n_byte(int fd, unsigned char *tx_buf)
{
    int ret;
    int i;
    ret = write(fd, tx_buf, 10);
    if (ret < 0){
        printf("send n byte error");
        return;
    }
    else {
        printf("send n byte ");
        for (i = 0; i < 10; i++){
		    printf("%.2X ", tx_buf[i]);
        }
        printf("success\n");
    }
}
void transfer_phone_byte(int fd, unsigned char *tx_buf)
{
    int ret;
	uint8_t tx[10] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF,0xFF
	};
	uint8_t rx[10];
	uint8_t MINSQ;
	MINSQ = tx_buf[2] >> 4;
	int i;
	tx[0] = tx_buf[0];
	tx[1] = tx_buf[1];
    tx[2] = tx_buf[2];
    for (i = 0; i < (MINSQ / 2);i++){
		tx[i+3] = tx_buf[i+3];
	}
    ret = write(fd, tx, 10);
    if (ret < 0){
        printf("send n byte error");
        return;
    }
    else {
        printf("gui so dien thoai ");
        for (i = 0; i < 10; i++){
		    printf("%.2X ", tx[i]);
        }
        printf("success\n");
    }
}
