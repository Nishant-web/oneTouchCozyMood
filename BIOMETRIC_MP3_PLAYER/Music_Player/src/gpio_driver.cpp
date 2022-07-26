/*
 * Code   : GPIO Driver : Source File [.cpp]
 */

#include "gpio_driver.h"

class GPIO_DRIVER;

GPIO_DRIVER::GPIO_DRIVER(void)
{
}

GPIO_DRIVER::~GPIO_DRIVER(void)
{
	close(gpio_fd_id);
}

uint8_t GPIO_DRIVER::gpio_init(uint8_t _gpio_pin, uint8_t gpio_dir)
{
	int fd_id;
	char buff[100];
	
	gpio_pin = _gpio_pin;

	fd_id = open("/sys/class/gpio/unexport", O_WRONLY);
	if(fd_id < 0)
		return(GPIO_ERR);
	sprintf(buff, "%d", gpio_pin);
	write(fd_id, buff, 2);
	usleep(100*1000);
	close(fd_id);

	fd_id = open("/sys/class/gpio/export", O_WRONLY);
	if(fd_id < 0)
		return(GPIO_ERR);
	write(fd_id, buff, 2);
	usleep(100*1000);
	close(fd_id);

	bzero(buff, sizeof(buff));
	sprintf(buff, "/sys/class/gpio/gpio%d/direction", gpio_pin);
	fd_id = open(buff, O_WRONLY);
	if(fd_id < 0)
		return(GPIO_ERR);

	bzero(buff, sizeof(buff));
	if(gpio_dir == GPIO_OUT)
		sprintf(buff, "out");
	else if(gpio_dir == GPIO_IN)
		sprintf(buff, "in");
	else
		return(GPIO_ERR);
	write(fd_id, buff, strlen(buff)+1);
	usleep(100*1000);
	close(fd_id);

	bzero(buff, sizeof(buff));
	sprintf(buff, "/sys/class/gpio/gpio%d/value", gpio_pin);
	gpio_fd_id = open(buff, O_RDWR);
	if(gpio_fd_id < 0)
		return(GPIO_ERR);
	usleep(100*1000);

	return(GPIO_OK);
}

uint8_t GPIO_DRIVER::gpio_read(void)
{
	char buff[5];

	lseek(gpio_fd_id, 0, SEEK_SET);
	read(gpio_fd_id, buff, 10);
	usleep(1000*20);

	if(buff[0] == '1')
		return(1);
	else
		return(0);
}

void GPIO_DRIVER::gpio_write(uint8_t state)
{
	char buff[5];

	sprintf(buff, "%d", state);
	lseek(gpio_fd_id, 0, SEEK_SET);
	write(gpio_fd_id, buff, 2);
	usleep(1000*20);
}
