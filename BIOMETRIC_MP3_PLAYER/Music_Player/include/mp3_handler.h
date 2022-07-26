/*
 * Code   : MP3 Handler (.h header)
 * Date   : 10/07/2022 @ 17:18
 */

#ifndef __MP3_HANDLER_H
#define __MP3_HANDLER_H

#include <ao/ao.h>
#include <mpg123.h>
#include <pthread.h>
#include <string.h>
#include <unistd.h>

#define BITS 8

class MP3
{
	private:
	bool play_flag;
    	unsigned char *buffer;
    	int err, driver_id, channels, encoding;
    	size_t buffer_size, done;
    	long rate;
    	ao_device *dev;
    	ao_sample_format format;
    	mpg123_handle *mpgh;
	pthread_t mp3_thread_id;

	public:
	MP3(void);
	void init(void);
	void load(const char*);
	void play(void);
	void stop(void);
	bool is_playing(void);
	void buffer_mp3(void);
	static void *mp3_thread_callback(void*);
};

#endif
