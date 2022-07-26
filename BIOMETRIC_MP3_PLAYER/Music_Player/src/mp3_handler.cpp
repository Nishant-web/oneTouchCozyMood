/*
 * Code   : MP3 Handler (.cpp source)
 * Date   : 10/07/2022 @ 17:10
 */

#include "mp3_handler.h"
  
class MP3;

MP3::MP3(void)
{
	play_flag = false;
    	pthread_create(&mp3_thread_id, NULL, MP3::mp3_thread_callback, this);
}

void MP3::init(void)
{
	ao_initialize();
	driver_id = ao_default_driver_id();
	mpg123_init();

	mpgh = mpg123_new(NULL, &err);
	buffer_size = mpg123_outblock(mpgh);
	buffer = (unsigned char*)malloc(buffer_size*sizeof(unsigned char));
}

void MP3::load(const char *file_name)
{
	mpg123_open(mpgh, file_name);
	mpg123_getformat(mpgh, &rate, &channels, &encoding);

	memset(&format, 0, sizeof(format));
	format.bits = mpg123_encsize(encoding) * 8;
	format.rate = rate;
	format.channels = channels;
	format.byte_format = AO_FMT_NATIVE;
	format.matrix = 0;
	dev = ao_open_live(driver_id, &format, NULL);
}

void MP3::play(void)
{
	play_flag = true;
}

void MP3::stop(void)
{
	play_flag = false;
}

bool MP3::is_playing(void)
{
	return(play_flag);
}

void *MP3::mp3_thread_callback(void *argp)
{
	MP3 *a = (MP3*)argp;
	for(;;)
	{
    		a->buffer_mp3();
	}
}

void MP3::buffer_mp3(void)
{
	if(play_flag)
	{
		if(mpg123_read(mpgh, buffer, buffer_size, &done) == MPG123_OK)
		{
			ao_play(dev, (char*)buffer, done);
		}
		else
		{
			play_flag = false;
		}
	}
	else
	{
		sleep(1);
	}
}
