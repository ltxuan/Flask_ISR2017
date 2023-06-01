#include "spi.h"

static void pabort(const char *s)
{
	perror(s);
	abort();
}
/* tranfer one byte */
void transfer(int fd, unsigned char byte)
{
	int ret;
	uint8_t tx[9] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF
	};
	uint8_t rx[9];
	tx[0] = byte;
 printf(" cac byte duoc gui di\n");
 for (ret = 0; ret < 9; ret++){
   if (!(ret % 6))
			puts("");
		printf("%.2X ", tx[ret]);
 }
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = 9,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
  printf("du lieu nhan duoc sau khi gui 9 byte\n");
  for (ret = 0; ret < 9; ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", rx[ret]);
	}
}
void transfer_n_byte(int fd, unsigned char *tx_buf)
{
	int ret;
	uint8_t tx[9] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF
	};
	uint8_t rx[9];
	uint8_t MINSQ;
	MINSQ = tx_buf[1] >> 4;
	int i;
	tx[0] = tx_buf[0];
	tx[1] = tx_buf[1];
	for (i = 0; i < (MINSQ / 2);i++){
		tx[i+2] = tx_buf[i+2];
	}
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = 9,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};

	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
  printf("gui so dien thoai xuong stm32\n");
	if (ret < 1)
		pabort("can't send spi message");
  for (ret = 0; ret < 9; ret++){
   if (!(ret % 6))
			puts("");
		printf("%.2X ", tx[ret]);
 }
}
/* read channel state and number phone from stm32 */
char read_spi(int fd , char *rd_buf, char *flag){
	int ret,i,j = 1;
	uint8_t channel, num_byte;
  transfer(fd, 0xFF);
  sleep(0.1);
	uint8_t tx[9] = {
		0xFF, 0xFF, 0xFF, 0xFF, 0xFF, 0xFF,
		0xFF, 0xFF, 0xFF
	};
	uint8_t rx[9];
	struct spi_ioc_transfer tr = {
		.tx_buf = (unsigned long)tx,
		.rx_buf = (unsigned long)rx,
		.len = 9,
		.delay_usecs = delay,
		.speed_hz = speed,
		.bits_per_word = bits,
	};
	ret = ioctl(fd, SPI_IOC_MESSAGE(1), &tr);
	if (ret < 1)
		pabort("can't send spi message");
  for (ret = 0; ret < 9; ret++) {
		if (!(ret % 6))
			puts("");
		printf("%.2X ", rx[ret]);
	}
	channel = rx[0] >> 3;
	*flag = rx[0] & 0x07;
	printf("read_spi channel = %d, flag = %d \n", channel, *flag);

	/* calling state get number from stm32*/
    if (*flag == 0x01){
    printf("if flag = 1\n");
		num_byte = rx[1] >> 4;
		rd_buf[0] = rx[1] & 0x0F;
		for(i = 0; i < num_byte / 2; i++){
            rd_buf[j] = rx[i+2] >> 4;
			      j++;
			if (j < num_byte){
				rd_buf[j] = rx[i+2] & 0x0F;
				j++;
			}
        }
    printf("so dien thoai nhan duoc tu stm32\n");
		for(i = 0; i< num_byte; i++){
            if (rd_buf[i] ==10) rd_buf[i] = 0;      //digit 0 in analog phone = 10
            printf("%d ",rd_buf[i]);
            rd_buf[i] += 48;
        }
	}
	return channel;
}
void init_gpio(){
    int fd;
    fd = open("/sys/class/gpio/export", O_WRONLY);
	write(fd, "340", 3);
	close(fd);

	//Set gpio25 as input
	fd = open("/sys/class/gpio/gpio340/direction", O_WRONLY);
	write(fd, "in", 2);
	close(fd);

	//Set gpio25 interrupt
	fd = open("/sys/class/gpio/gpio340/edge", O_WRONLY);
	//write(fd1, "falling", 7);
	write(fd, "rising", 6);
	close(fd);
}
int init_spi(){
    int ret = 0;
	int fd;

	fd = open(device, O_RDWR);
	if (fd < 0)
		pabort("can't open device");

	/*
	 * spi mode
	 */
	ret = ioctl(fd, SPI_IOC_WR_MODE, &mode);
	if (ret == -1)
		pabort("can't set spi mode");

	ret = ioctl(fd, SPI_IOC_RD_MODE, &mode);
	if (ret == -1)
		pabort("can't get spi mode");

	/*
	 * bits per word
	 */
	ret = ioctl(fd, SPI_IOC_WR_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't set bits per word");

	ret = ioctl(fd, SPI_IOC_RD_BITS_PER_WORD, &bits);
	if (ret == -1)
		pabort("can't get bits per word");

	/*
	 * max speed hz
	 */
	ret = ioctl(fd, SPI_IOC_WR_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't set max speed hz");

	ret = ioctl(fd, SPI_IOC_RD_MAX_SPEED_HZ, &speed);
	if (ret == -1)
		pabort("can't get max speed hz");

	printf("spi mode: %d\n", mode);
	printf("bits per word: %d\n", bits);
	printf("max speed: %d Hz (%d KHz)\n", speed, speed/1000);
    return fd;
}