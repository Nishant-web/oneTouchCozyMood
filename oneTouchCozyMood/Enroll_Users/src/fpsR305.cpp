/*
 * Library : R305 Finger Print Sensor
 * File    : fpsR305.cpp [source]
 */

#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "fpsR305.h"

/* Class declaration */
class FPS_R305;

/* FPS_R305::FPS_R305(void) {...}
 * ...
 * @brief  : Non-parameterized constructor member of class FPS_R305.
 * @param  : None/void
 * @retval : None/void
 */
FPS_R305::FPS_R305(void)
{
	/* ... */
}

/* FPS_R305::FPS_R305(const char*, unsigned int) {...}
 * ...
 * @brief  : Parameterized constructor member of class FPS_R305 to initilize data members.
 * @param  : const char* (serial/UART/TTL port devfs directory), unsigned int (UART Bauderate)
 * @retval : None/void
 */
FPS_R305::FPS_R305(const char *_serial, unsigned int _br)
{
	fpsr305_serial_port_name = _serial;
	fpsr305_serial_port_br = _br;
}

/* int8_t FPS_R305::get_resp(uint8_t*, uint8_t) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to get command respone from sensor.
 * @param  : uint8_t* (cmd buffer/array pointer), uint8_t (cmd buffer length)
 * @retval : int8_t [signed integer 8-bit] (Returns response code from sensor on success else -1).
 */
int8_t FPS_R305::get_resp(uint8_t *_buff, uint8_t _bytes)
{
	uint16_t csum;
	if(!get_bytes(_buff, _bytes))
		return(-1);
	csum = get_sum(_buff, 6, _bytes-8);
	if(csum == (((_buff[_bytes-2]) << 8) | _buff[_bytes-1]))
		return(_buff[9]);
	else
		return(-1);
}

/* bool FPS_R305::init(void) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to initialize sensor module.
 * @param  : None/void
 * @retval : bool (Returns true on success else false).
 */
bool FPS_R305::init(void)
{
	uint8_t buff[16];
	uint16_t csum;

	/* Initializing wiringPi setup */
	if(wiringPiSetup() < 0)
		return(false);

	/* Opening chipset's serial/UART/TTL port to communicate with sensor*/
	fpsr305_serial_port_id = serialOpen(fpsr305_serial_port_name, fpsr305_serial_port_br);
	if(fpsr305_serial_port_id < 0)
		return(false);
	
	/* Few attempts to set password of sensor module */
	set_password(0xFFFFFFFF);delay(100);
	set_password(0xFFFFFFFF);delay(100);
	set_password(0xFFFFFFFF);delay(100);
	set_password(0xFFFFFFFF);delay(100);

	// Header
	buff[0] = 0xEF;
	buff[1] = 0x01;

	// Address
	buff[2] = 0xFF;
	buff[3] = 0xFF;
	buff[4] = 0xFF;
	buff[5] = 0xFF;

	// Package Id
	buff[6] = 0x01;

	// Package Length
	buff[7] = 0x00;
	buff[8] = 0x07;

	// Instruction Code
	buff[9] = 0x13;

	// Password
	buff[10] = 0xFF;
	buff[11] = 0xFF;
	buff[12] = 0xFF;
	buff[13] = 0xFF;

	// Checksum
	csum = get_sum(buff, 6, 8);
	buff[14] = ((csum >> 8) & 0xFF);
	buff[15] = (csum & 0xFF);

	/* Clearing serial Tx & Rx buffers */
	serialFlush(fpsr305_serial_port_id);
	/* Transmit bytes to sensor module */
	send_bytes(buff, 16);

	/* Check response from sensor */
	if(get_resp(buff, 12) == 0x00)
		return(true);
	else
		return(false);
}

/* void FPS_R305::set_password(uint32_t) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to set password of sensor module.
 * @param  : uint32_t (32-Bit sensor module password)
 * @retval : None/void
 */
void FPS_R305::set_password(uint32_t _pw)
{
	/* Local variables */
	uint8_t buff[16];
	uint16_t csum;

	// Header
	buff[0] = 0xEF;
	buff[1] = 0x01;
	// Address
	buff[2] = 0xFF;
	buff[3] = 0xFF;
	buff[4] = 0xFF;
	buff[5] = 0xFF;
	// Package Id
	buff[6] = 0x01;
	// Package Length
	buff[7] = 0x00;
	buff[8] = 0x07;
	// Instruction Code
	buff[9] = 0x12;
	// Password
	buff[10] = 0xFF;
	buff[11] = 0xFF;
	buff[12] = 0xFF;
	buff[13] = 0xFF;

	// Checksum
	csum = get_sum(buff, 6, 8);
	buff[14] = ((csum >> 8) & 0xFF);
	buff[15] = (csum & 0xFF);

	/* Clearing serial Tx & Rx buffers */
	serialFlush(fpsr305_serial_port_id);
	/* Transmit bytes to sensor module */
	send_bytes(buff, 16);
}

/* bool FPS_R305::GenImg(void) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to collect finger image from sensor module.
 * @param  : None/void
 * @retval : bool (Return true on success else false)
 */
bool FPS_R305::GenImg(void)
{
	/* Local variables */
	uint8_t buff[13];
	uint16_t csum;

	// Collect Finger Image :-
	// Header
	buff[0] = 0xEF;
	buff[1] = 0x01;
	// Address
	buff[2] = 0xFF;
	buff[3] = 0xFF;
	buff[4] = 0xFF;
	buff[5] = 0xFF;
	// Package Id
	buff[6] = 0x01;
	// Package Length
	buff[7] = 0x00;
	buff[8] = 0x03;
	// Instruction Code
	buff[9] = 0x01;
	// Checksum
	csum = get_sum(buff, 6, 4);
	buff[10] = ((csum >> 8) & 0xFF);
	buff[11] = (csum & 0xFF);

	/* Clearing serial Tx & Rx buffers */
	serialFlush(fpsr305_serial_port_id);
	/* Transmit bytes to sensor module */
	send_bytes(buff, 12);

	/* Check response from sensor */
	if(get_resp(buff, 12) != 0x00)
		return(false);
	return(true);
}

/* bool FPS_R305::Img2Tz(uint8_t) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to generate character file from captured finger image.
 * @param  : uint8_t (Character buffer ID)
 * @retval : bool (Return true on success else false)
 */
bool FPS_R305::Img2Tz(uint8_t i)
{
	/* Local variables */
	uint8_t buff[14];
	uint16_t csum;

	// Generate Character File :-
	// Header
	buff[0] = 0xEF;
	buff[1] = 0x01;
	// Address
	buff[2] = 0xFF;
	buff[3] = 0xFF;
	buff[4] = 0xFF;
	buff[5] = 0xFF;
	// Package Id
	buff[6] = 0x01;
	// Package Length
	buff[7] = 0x00;
	buff[8] = 0x04;
	// Instruction Code
	buff[9] = 0x02;
	// Buffer ID
	buff[10] = i;
	// Checksum
	csum = get_sum(buff, 6, 5);
	buff[11] = ((csum >> 8) & 0xFF);
	buff[12] = (csum & 0xFF);

	/* Clearing serial Tx & Rx buffers */
	serialFlush(fpsr305_serial_port_id);
	/* Transmit bytes to sensor module */
	send_bytes(buff, 13);

	/* Check response from sensor */
	if(get_resp(buff, 12) != 0x00)
		return(false);

	return(true);
}

/* bool FPS_R305::RegModel(void) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to generate template from captured data.
 * @param  : None/void
 * @retval : bool (Return true on success else false)
 */
bool FPS_R305::RegModel(void)
{
	/* Local variables */
	uint8_t buff[14];
	uint16_t csum;

	// Generate Template :-
	// Header
	buff[0] = 0xEF;
	buff[1] = 0x01;
	// Address
	buff[2] = 0xFF;
	buff[3] = 0xFF;
	buff[4] = 0xFF;
	buff[5] = 0xFF;
	// Package Id
	buff[6] = 0x01;
	// Package Length
	buff[7] = 0x00;
	buff[8] = 0x03;
	// Instruction Code
	buff[9] = 0x05;
	// Checksum
	csum = get_sum(buff, 6, 4);
	buff[10] = ((csum >> 8) & 0xFF);
	buff[11] = (csum & 0xFF);

	/* Clearing serial Tx & Rx buffers */
	serialFlush(fpsr305_serial_port_id);
	/* Transmit bytes to sensor module */
	send_bytes(buff, 12);
	
	/* Check response from sensor */
	if(get_resp(buff, 12) != 0x00)
		return(false);

	return(true);
}

/* bool FPS_R305::Store(uint16_t) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to store template into sensor module.
 * @param  : uint16_t (Finger print template ID)
 * @retval : bool (Return true on success else false)
 */
bool FPS_R305::Store(uint16_t _id)
{
	/* Local variables */
	uint8_t buff[16];
	uint16_t csum;

	// Store Template :-
	// Header
	buff[0] = 0xEF;
	buff[1] = 0x01;
	// Address
	buff[2] = 0xFF;
	buff[3] = 0xFF;
	buff[4] = 0xFF;
	buff[5] = 0xFF;
	// Package Id
	buff[6] = 0x01;
	// Package Length
	buff[7] = 0x00;
	buff[8] = 0x06;
	// Instruction Code
	buff[9] = 0x06;
	// Buffer ID
	buff[10] = 0x01;
	// Page ID
	buff[11] = ((_id >> 8) & 0xFF);
	buff[12] = (_id & 0xFF);
	// Checksum
	csum = get_sum(buff, 6, 7);
	buff[13] = ((csum >> 8) & 0xFF);
	buff[14] = (csum & 0xFF);

	/* Clearing serial Tx & Rx buffers */
	serialFlush(fpsr305_serial_port_id);
	/* Transmit bytes to sensor module */
	send_bytes(buff, 15);

	/* Check response from sensor */
	if(get_resp(buff, 12) == 0x00)
		return(true);

	return(false);
}

/* bool FPS_R305::enroll_id(uint16_t) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to enroll user finger print into sensor module.
 * @param  : uint16_t (Finger print ID)
 * @retval : bool (Return true on success else false)
 */
bool FPS_R305::enroll_id(uint16_t _id)
{
	/* Local variable */
	bool resp;

	resp = false;
	while(!resp)
		resp = GenImg();
	delay(100);

	if(!Img2Tz(1))
		return(false);
	delay(100);
	
	resp = false;
	while(!resp)
		resp = GenImg();
	delay(100);

	if(!Img2Tz(2))
		return(false);
	delay(100);

	if(!RegModel())
		return(false);
	delay(100);

	if(!Store(_id))
		return(false);
	delay(100);
	return(true);
}

/* int16_t FPS_R305::scan_id(void) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to scan finger print from sensor module.
 * @param  : None/void
 * @retval : int16_t [signed integer 16-bit] (Return finger ID on success else -1)
 */
int16_t FPS_R305::scan_id(void)
{
	uint8_t buff[18];
	uint16_t csum, id;

	// Collect Finger Image
	if(!GenImg())
		return(-1);

	if(!Img2Tz(1))
		return(-1);

	// Search :-
	// Header
	buff[0] = 0xEF;
	buff[1] = 0x01;
	// Address
	buff[2] = 0xFF;
	buff[3] = 0xFF;
	buff[4] = 0xFF;
	buff[5] = 0xFF;
	// Package Id
	buff[6] = 0x01;
	// Package Length
	buff[7] = 0x00;
	buff[8] = 0x08;
	// Instruction Code
	buff[9] = 0x04;
	// Buffer ID
	buff[10] = 0x01;
	// Start Page
	buff[11] = 0x00;
	buff[12] = 0x00;
	// Page Number
	buff[13] = 0x03;
	buff[14] = 0xE8;
	// Checksum
	csum = get_sum(buff, 6, 9);
	buff[15] = ((csum >> 8) & 0xFF);
	buff[16] = (csum & 0xFF);

	/* Clearing serial Tx & Rx buffers */
	serialFlush(fpsr305_serial_port_id);
	/* Transmit bytes to sensor module */
	send_bytes(buff, 17);

	/* Check response from sensor */
	if(get_resp(buff, 16) != 0x00)
		return(-1);

	id = (buff[11] & 0xFF);
	id |= (buff[10] << 8);
	return(id);
}

/* bool FPS_R305::delete_id(uint16_t) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to delete finger print template from sensor module.
 * @param  : uint16_t (Finger print ID)
 * @retval : bool (Return true on success else false)
 */
bool FPS_R305::delete_id(uint16_t _id)
{
	/* Local variables */
	uint8_t buff[17];
	uint16_t csum;

	// Header
	buff[0] = 0xEF;
	buff[1] = 0x01;
	// Address
	buff[2] = 0xFF;
	buff[3] = 0xFF;
	buff[4] = 0xFF;
	buff[5] = 0xFF;
	// Package Id
	buff[6] = 0x01;
	// Package Length
	buff[7] = 0x00;
	buff[8] = 0x07;
	// Instruction Code
	buff[9] = 0x0C;
	// Page Number
	buff[10] = (_id >> 8) && 0xFF;
	buff[11] = (_id & 0xFF);
	// Number of templates
	buff[12] = 0x00;
	buff[13] = 0x01;
	// Checksum
	csum = get_sum(buff, 6, 8);
	buff[14] = ((csum >> 8) & 0xFF);
	buff[15] = (csum & 0xFF);

	/* Clearing serial Tx & Rx buffers */
	serialFlush(fpsr305_serial_port_id);
	/* Transmit bytes to sensor module */
	send_bytes(buff, 16);

	/* Check response from sensor */
	if(get_resp(buff, 12) == 0x00)
		return(true);
	else
		return(false);
}

/* bool FPS_R305::delete_all(void) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to delete all finger print templates from sensor module.
 * @param  : None/void
 * @retval : bool (Return true on success else false)
 */
bool FPS_R305::delete_all(void)
{
	/* Local variables */
	uint8_t buff[12];
	uint16_t csum;

	// Header
	buff[0] = 0xEF;
	buff[1] = 0x01;
	// Address
	buff[2] = 0xFF;
	buff[3] = 0xFF;
	buff[4] = 0xFF;
	buff[5] = 0xFF;
	// Package Id
	buff[6] = 0x01;
	// Package Length
	buff[7] = 0x00;
	buff[8] = 0x03;
	// Instruction Code
	buff[9] = 0x0D;
	// Checksum
	csum = get_sum(buff, 6, 4);
	buff[10] = ((csum >> 8) & 0xFF);
	buff[11] = (csum & 0xFF);

	/* Clearing serial Tx & Rx buffers */
	serialFlush(fpsr305_serial_port_id);
	/* Transmit bytes to sensor module */
	send_bytes(buff, 12);

	/* Check response from sensor */
	if(get_resp(buff, 12) == 0x00)
		return(true);
	else
		return(false);
}

/* void FPS_R305::send_bytes(uint8_t*, uint16_t) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to transmit bytes to sensor module.
 * @param  : uint8_t* (Tx Buffer/array pointer), uint16_t (Tx Buffer/array length)
 * @retval : None/void
 */
void FPS_R305::send_bytes(uint8_t *_buff, uint16_t _buff_len)
{
	for(uint16_t i = 0 ; i < _buff_len ; i++)
		serialPutchar(fpsr305_serial_port_id, _buff[i]); /* Transmitting byte-by-byte */
}

/* bool FPS_R305::get_bytes(uint8_t*, uint16_t) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to receive bytes from sensor module.
 * @param  : uint8_t* (Rx Buffer/array pointer), uint16_t (Rx Buffer/array length)
 * @retval : bool (Returns true on success else false)
 */
bool FPS_R305::get_bytes(uint8_t *_buff, uint16_t _l)
{
	/* Local variables */
	unsigned long timeout_millis = millis();
	/* Check for available databytes */
	while(serialDataAvail(fpsr305_serial_port_id) < _l && (millis() - timeout_millis) < 1500);

	/* return false on timeout */
	if((millis() - timeout_millis) >= 1500)
		return(false);

	/* Buffering received data bytes from sensor */
	for(uint16_t i = 0 ; i < _l ; i++)
		_buff[i] = serialGetchar(fpsr305_serial_port_id); /* byte-by-byte reception */
	return(true);
}

/* uint16_t FPS_R305::get_sum(uint8_t*, uint16_t, uint16_t) {...}
 * ...
 * @brief  : Member function of class FPS_R305 to sum buffer elements from specific offset.
 * @param  : uint8_t* (Buffer/array pointer), uint16_t (Buffer/array offset), uint16_t (Buffer/array length)
 * @retval : uint16_t (Returns sum of elements of buffer/array from offset)
 */
uint16_t FPS_R305::get_sum(uint8_t *_buff, uint16_t _ofs, uint16_t _l)
{
	uint16_t _sum = 0;
	while(_l--)
		_sum += _buff[(_ofs+_l)];
	return(_sum);
}
