/*
 * Code   : GPIO Driver : Header File [.h]
 */

#ifndef __GPIO_DRIVER_H__
#define __GPIO_DRIVER_H__

#include <fcntl.h>
#include <unistd.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define GPIO_OUT  0
#define GPIO_IN   1
#define GPIO_ERR  0
#define GPIO_OK   1

class GPIO_DRIVER
{
	private:
	int gpio_fd_id;
	uint8_t gpio_pin;

	public:
	~GPIO_DRIVER(void);
	GPIO_DRIVER(void);
	uint8_t gpio_init(uint8_t, uint8_t);
	uint8_t gpio_read(void);
	void gpio_write(uint8_t);
};

#endif
