/*
 * Code   : GPIO Driver : Source File [.cpp]
 */

#include "gpio_driver.h"

/* GPIO Class Declaration */
class GPIO_DRIVER;

/* GPIO_DRIVER::GPIO_DRIVER(void) {...}
 * ...
 * @brief  : Constructor member of class GPIO_DRIVER.
 * @param  : None/void
 * @retval : None/void
 */
GPIO_DRIVER::GPIO_DRIVER(void)
{
}

/* GPIO_DRIVER::~GPIO_DRIVER(void) {...}
 * ...
 * @brief  : Destructor member of class GPIO_DRIVER to close sysfs file and delete object.
 * @param  : None/void
 * @retval : None/void
 */
GPIO_DRIVER::~GPIO_DRIVER(void)
{
	/* Closing GPIO sysfs file */
	close(gpio_fd_id);
}

/* uint8_t GPIO_DRIVER::gpio_init(uint8_t, uint8_t) {...}
 * ...
 * @brief  : Member function of class GPIO_DRIVER to initialize and configure data members & GPIO sysfs.
 * @param  : uint8_t (GPIO logical pin no.), uint8_t (GPIO direction IN/OUT)
 * @retval : uint8_t (Returns GPIO_OK resp/macro on success else GPIO_ERR)
 */
uint8_t GPIO_DRIVER::gpio_init(uint8_t _gpio_pin, uint8_t gpio_dir)
{
	/* Local varibales */
	int fd_id;
	char buff[100];
	
	/* Coping logical GPIO pin number into data member */
	gpio_pin = _gpio_pin;

	/* Un-exporting/Un-register particular GPIO from GPIO class in sysfs */
	fd_id = open("/sys/class/gpio/unexport", O_WRONLY);
	if(fd_id < 0)
		return(GPIO_ERR);
	sprintf(buff, "%d", gpio_pin);
	write(fd_id, buff, 2);
	usleep(100*1000);
	close(fd_id);

	/* Exporting/Registering particular GPIO from GPIO class in sysfs */
	fd_id = open("/sys/class/gpio/export", O_WRONLY);
	if(fd_id < 0)
		return(GPIO_ERR);
	write(fd_id, buff, 2);
	usleep(100*1000);
	close(fd_id);

	/* Configuring GPIO direction (Input or Output) */
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

	/* Opening GPIO value file from sysfs inorder to do read/write operation on GPIOs */
	bzero(buff, sizeof(buff));
	sprintf(buff, "/sys/class/gpio/gpio%d/value", gpio_pin);
	gpio_fd_id = open(buff, O_RDWR);
	if(gpio_fd_id < 0)
		return(GPIO_ERR);
	usleep(100*1000);

	/* Return GPIO_OK response on success */
	return(GPIO_OK);
}

/* uint8_t GPIO_DRIVER::gpio_read(void) {...}
 * ...
 * @brief  : Member function of class GPIO_DRIVER to read state of GPIO.
 * @param  : None/void
 * @retval : uint8_t (Returns GPIO state (1 or 0))
 */
uint8_t GPIO_DRIVER::gpio_read(void)
{
	/* Array to get GPIO I/O value file content from sysfs */
	char buff[5];

	/* Seeking file pointer location to start of file */
	lseek(gpio_fd_id, 0, SEEK_SET);
	/* Copying file content into buffer/array */
	read(gpio_fd_id, buff, 10);
	/* Some delay for proper read operation */
	usleep(1000*20);

	/* Checking 1st char array */
	if(buff[0] == '1')
		return(1); /* Return 1 if GPIO state is high */
	else
		return(0); /* Return 0 if GPIO state is low */
}

/* void GPIO_DRIVER::gpio_write(uint8_t) {...}
 * ...
 * @brief  : Member function of class GPIO_DRIVER to write state to GPIO.
 * @param  : uint8_t (GPIO state (1 or 0))
 * @retval : None/void
 */
void GPIO_DRIVER::gpio_write(uint8_t state)
{
	/* Array to get GPIO I/O value file content from sysfs */
	char buff[5];

	/* Copying state value into array as a string */
	sprintf(buff, "%d", state);
	/* Seeking file pointer location to start of file */
	lseek(gpio_fd_id, 0, SEEK_SET);
	/* Copying array/buffer content to value file of GPIO in sysfs */
	write(gpio_fd_id, buff, 2);
	/* Some delay for proper write operation */
	usleep(1000*20);
}
