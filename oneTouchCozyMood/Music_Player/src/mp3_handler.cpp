/*
 * Code   : MP3 Handler (.cpp source)
 * Date   : 10/07/2022 @ 17:10
 */

#include "mp3_handler.h"

/* Class Declaration */  
class MP3;

/* MP3::MP3(void) {...}
 * ...
 * @brief  : Constructor member of class MP3 to initialize data member and create MP3 buffering thread.
 * @param  : None/void
 * @retval : None/void
 */
MP3::MP3(void)
{
	/* Intilize PLAY state to false */
	play_flag = false;
	/* Create thread for MP3 buffering */
    pthread_create(&mp3_thread_id, NULL, MP3::mp3_thread_callback, this);
}

/* void MP3::init(void) {...}
 * ...
 * @brief  : Member function of class MP3 to initialize data members, MPEG codec, & PCM audio driver
 * @param  : None/void
 * @retval : None/void
 */
void MP3::init(void)
{
	/* Initialize on-board SOC's PCM audio driver */
	ao_initialize();
	/* Get PCM driver ID */
	driver_id = ao_default_driver_id();
	/* Initlialize MP3 decoder */
	mpg123_init();

	/* Get mpeg handler object */
	mpgh = mpg123_new(NULL, &err);
	/* Get buffer size */
	buffer_size = mpg123_outblock(mpgh);
	/* Allocate dynamic buffer/array for MP3 buffering */
	buffer = (unsigned char*)malloc(buffer_size*sizeof(unsigned char));
}

/* void MP3::load(const char*) {...}
 * ...
 * @brief  : Member function of class MP3 to load, initilize, and get format of input .mp3 file.
 * @param  : const char* (mp3 filename)
 * @retval : None/void
 */
void MP3::load(const char *file_name)
{
	/* Open mp3 file for decoding */
	mpg123_open(mpgh, file_name);
	/* Get mp3 file parameters */
	mpg123_getformat(mpgh, &rate, &channels, &encoding);

	/* Clear format object */
	memset(&format, 0, sizeof(format));
	/* Setting format object with mp3 file parameters */
	format.bits = mpg123_encsize(encoding) * BITS;
	format.rate = rate;
	format.channels = channels;
	format.byte_format = AO_FMT_NATIVE;
	format.matrix = 0;
	/* Get PCM device object */
	dev = ao_open_live(driver_id, &format, NULL);
}

/* void MP3::play(void) {...}
 * ...
 * @brief  : Member function of class MP3 to set play flag.
 * @param  : None/void
 * @retval : None/void
 */
void MP3::play(void)
{
	/* Setting play flag to true will buffer mp3 samples to audio driver via thread */
	play_flag = true;
}

/* void MP3::stop(void) {...}
 * ...
 * @brief  : Member function of class MP3 to clear play flag.
 * @param  : None/void
 * @retval : None/void
 */
void MP3::stop(void)
{
	/* Clearing play flag to will stop buffer of mp3 samples */
	play_flag = false;
}

/* bool MP3::is_playing(void) {...}
 * ...
 * @brief  : Member function of class MP3 to get status of player.
 * @param  : None/void
 * @retval : bool (Returns true if already playing else false)
 */
bool MP3::is_playing(void)
{
	return(play_flag); /* Return play_flag */
}

/* void *MP3::mp3_thread_callback(void*) {...}
 * ...
 * @brief  : Member thread callback function of class MP3 to invoke buffering function simultenously.
 * @param  : void* (void pointer to arguments of thread method)
 * @retval : void* (returns void pointer after termination of thread callback)
 */
void *MP3::mp3_thread_callback(void *argp)
{
	/* Typecasting void pointer to this class type */
	MP3 *a = (MP3*)argp;
	/* Infinite loop */
	for(;;)
	{
		/* Calling buffering function simultenously */
    	a->buffer_mp3();
	}
}

/* void MP3::buffer_mp3(void) {...}
 * ...
 * @brief  : Member function of class MP3 for mp3 samples buffering.
 * @param  : None/void
 * @retval : None/void
 */
void MP3::buffer_mp3(void)
{
	/* Checking is play flag is true */
	if(play_flag)
	{
		/* Reading (Mono/Stereo) samples from mpeg handler object into buffer */
		if(mpg123_read(mpgh, buffer, buffer_size, &done) == MPG123_OK)
		{
			/* Play collected samples via PCM driver object */
			ao_play(dev, (char*)buffer, done);
		}
		else
		{
			/* Setting play flag to false when done */
			play_flag = false;
		}
	}
	else
	{
		/* No need to do anything if play flag is false */
		/* Calling sleep api for more pre-emptions */
		sleep(1);
	}
}
