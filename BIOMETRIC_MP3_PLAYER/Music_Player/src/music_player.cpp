/*
 * Code   : Biometric Music Player
 * Date   : 10/07/2022 @ 17:48
 */

#include "player_gui.h"
#include "fpsR305.h"
#include "mp3_handler.h"
#include "dir_handler.h"
#include "gpio_driver.h"

#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>

#define DISPLAY_WIDTH   800
#define DISPLAY_HEIGHT  800
#define BG_COLOR        0x000F0F00

#define RED_LED_PIN     1
#define GREEN_LED_PIN   24
#define BLUE_LED_PIN    23
#define ONOFF_BUT_PIN   21
#define PLYPS_BUT_PIN   20
#define STOP_BUT_PIN    16

#define SYSTEM_OFF      0
#define SYSTEM_ON       1
#define MODE_PLAY       0
#define MODE_PAUSE      1
#define MODE_STOP       2

const uint8_t RGB_LED_PINS[3] = {RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN};
const uint8_t BUTTON_PINS[3] = {ONOFF_BUT_PIN, PLYPS_BUT_PIN, STOP_BUT_PIN};

PLAYER_GUI pgui(DISPLAY_WIDTH, DISPLAY_HEIGHT, BG_COLOR);
FPS_R305 FPS("/dev/serial0", 57600);
GPIO_DRIVER rgb_leds[3];
GPIO_DRIVER buttons[3];
DIR_HANDLER dhand;
MP3 mp3;

uint8_t io_init(void);
long long get_millis(void);

int main(int argc, char *argv[])
{
	int fps_id, user_id, user_dir_count, user_playlist_count = 0, playlist_counter = 0;
	char **user_dir_list, **user_playlist, user_name[30], playlist_dir[30];
	uint8_t system_status = SYSTEM_OFF, playing_mode = MODE_STOP;
	long long hb_led_millis = 0, play_led_millis = 0, gui_update_millis = 0, fps_scan_millis = 0;

	if(io_init() == 0)
		exit(1);

	mp3.init();

	dhand.load("../../data/");
	user_dir_count = dhand.get_count();
	user_dir_list = dhand.get_listing();

	sprintf(user_name, "...");
	user_id = 0;

	while(1)
	{

		if(buttons[0].gpio_read() == 1)
		{
			while(buttons[0].gpio_read() == 1) usleep(40*1000);
			system_status = !system_status;

			if(system_status == SYSTEM_OFF)
			{
				mp3.stop();
				pgui.clear_all();
			}
		}
		
		if(system_status == SYSTEM_ON)
		{

			if(buttons[1].gpio_read() == 1)
			{
				while(buttons[1].gpio_read() == 1) usleep(40*1000);
				if(playing_mode == MODE_PAUSE)
					playing_mode = MODE_PLAY;
				else if(playing_mode == MODE_PLAY)
					playing_mode = MODE_PAUSE;
			}

			if(buttons[2].gpio_read() == 1)
			{
				while(buttons[2].gpio_read() == 1) usleep(40*1000);
				playing_mode = MODE_STOP;
			}

			if(playing_mode == MODE_PAUSE && mp3.is_playing() == true)
			{
				mp3.stop();
			}
			else if(playing_mode == MODE_PLAY && mp3.is_playing() == false && playlist_counter > 0)
			{
				mp3.play();
			}
			else if(playing_mode == MODE_STOP)
			{
				mp3.stop();
				pgui.clear_all();
				playlist_counter = 0;
			}

			if((get_millis() - gui_update_millis) > 300 && playing_mode != MODE_STOP)
			{
				pgui.set_user_info(user_name, user_id, 0x00ff0000);
				pgui.set_playlist(user_playlist, user_playlist_count, 0x00ffff00);

				if(mp3.is_playing() == true && playlist_counter > 0 && playing_mode == MODE_PLAY)
				{
					pgui.set_playing(user_playlist[playlist_counter-1], 0xffffff00);
				}
				else
				{
					pgui.set_playing("", 0);
				}

				gui_update_millis = get_millis();
			}

			if((get_millis() - fps_scan_millis) > 400)
			{
				fps_id = FPS.scan_id();
				fps_scan_millis = get_millis();
			}

			if(fps_id > 0)
			{
				for(int i = 0 ; i < user_dir_count ; i++)
				{
					int id;
					char dum;

					sscanf(user_dir_list[i], "%d%c%s", &id, &dum, user_name);

					if(id == fps_id)
					{
						if(mp3.is_playing())
							mp3.stop();

						user_id = id;
						playlist_counter = 0;
						sprintf(playlist_dir, "../../data/%s/", user_dir_list[i]);
					
						dhand.load(playlist_dir);
						user_playlist_count = dhand.get_count();

						user_playlist = dhand.get_listing();
						playing_mode = MODE_PLAY;

						break;
					}
				}
			}

			if(user_playlist_count > 0 && playlist_counter < user_playlist_count)
			{
				if(mp3.is_playing() == false && playing_mode == MODE_PLAY)
				{
					char track[40];

					sprintf(track, "%s%s", playlist_dir, user_playlist[playlist_counter]);
					mp3.load(track);
					mp3.play();

					playlist_counter++;
				}
			}
			
			if(playing_mode == MODE_PLAY)
			{
				rgb_leds[0].gpio_write(0);

				if((get_millis() - play_led_millis) > 80)
				{
					rgb_leds[2].gpio_write(!rgb_leds[2].gpio_read());
					play_led_millis = get_millis();
				}
			}
			else if(playing_mode == MODE_PAUSE)
			{
				rgb_leds[0].gpio_write(0);
				rgb_leds[2].gpio_write(1);
			}
			else if(playing_mode == MODE_STOP)
			{
				rgb_leds[0].gpio_write(1);
				rgb_leds[2].gpio_write(0);
			}

			if((get_millis() - hb_led_millis) >= 2000)
			{
				rgb_leds[1].gpio_write(1);
				usleep(10*1000);
				rgb_leds[1].gpio_write(0);
				hb_led_millis = get_millis();
			}
		}
		else if(system_status == SYSTEM_OFF)
		{
			rgb_leds[1].gpio_write(0);
			rgb_leds[2].gpio_write(0);

			if((get_millis() - hb_led_millis) >= 1000)
			{
				rgb_leds[0].gpio_write(!rgb_leds[0].gpio_read());
				hb_led_millis = get_millis();
			}
		}

	}

	return(0);
}


uint8_t io_init(void)
{
	uint8_t i;

	if(wiringPiSetup() < 0)
	{
		std::cout << "Wiring Pi Error!!\n";
		return(0);
	}

	std::cout << "\nInitializing FPS R305 ";
	while(!FPS.init())
	{
		std::cout << ".";
		delay(100);
	}
	std::cout << "\nFPS R305 Initialized.\n";

	for(i = 0 ; i < 3 ; i++)
	{
		if(rgb_leds[i].gpio_init(RGB_LED_PINS[i], GPIO_OUT) == GPIO_ERR)
			return(0);
		if(buttons[i].gpio_init(BUTTON_PINS[i], GPIO_IN) == GPIO_ERR)
			return(0);
	}

	return(1);
}

long long get_millis(void)
{
	struct timeval t;
	long long millis;

	gettimeofday(&t, NULL);
	millis = (t.tv_sec*1000LL) + (t.tv_usec/1000);
	return millis;
}
