/*
 * Code   : MP3 Handler (.h header)
 * Date   : 13/04/2022 @ 01:37 AM
 */

#ifndef __MP3_HANDLER_H
#define __MP3_HANDLER_H

#include <ao/ao.h>
#include <mpg123.h>
#include <pthread.h>

#define BITS 8

class MP3
{
	private:
	bool play_flag, playing_flag;
    	unsigned char *buffer;
    	size_t buffer_size, done;
    	int err, driver, channels, encoding;
    	long rate;
    	ao_device *dev;
    	ao_sample_format format;
    	mpg123_handle *mh;
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
