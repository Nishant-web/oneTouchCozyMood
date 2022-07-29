/*
 * Code   : Biometric Music Player
 * Date   : 10/07/2022 @ 17:48
 */

/* User Defined Libraries */
#include "player_gui.h"
#include "fpsR305.h"
#include "mp3_handler.h"
#include "dir_handler.h"
#include "gpio_driver.h"

/* Standard Libraries */
#include <iostream>
#include <unistd.h>
#include <sys/time.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

/* GUI Parameters/Macros */
#define DISPLAY_WIDTH   800
#define DISPLAY_HEIGHT  800
#define BG_COLOR        0x000F0F00

/* GPIO Pin Macros */
#define RED_LED_PIN     1
#define GREEN_LED_PIN   24
#define BLUE_LED_PIN    23
#define ONOFF_BUT_PIN   21
#define PLYPS_BUT_PIN   20
#define STOP_BUT_PIN    16

/* System Macros */
#define SYSTEM_OFF      0
#define SYSTEM_ON       1
#define MODE_PLAY       0
#define MODE_PAUSE      1
#define MODE_STOP       2

/* GPIO Grouping/Arrays for LEDs and Buttons */
const uint8_t RGB_LED_PINS[3] = {RED_LED_PIN, GREEN_LED_PIN, BLUE_LED_PIN};
const uint8_t BUTTON_PINS[3] = {ONOFF_BUT_PIN, PLYPS_BUT_PIN, STOP_BUT_PIN};

/* Class objects declarations */
PLAYER_GUI pgui(DISPLAY_WIDTH, DISPLAY_HEIGHT, BG_COLOR);
FPS_R305 FPS("/dev/serial0", 57600);
GPIO_DRIVER rgb_leds[3];
GPIO_DRIVER buttons[3];
DIR_HANDLER dhand;
MP3 mp3;

/* Function Declarations */
void init(void);
long long get_millis(void);

int main(int argc, char *argv[])
{
	/* Local members/variables of main */
	int fps_id, user_id, user_dir_count, user_playlist_count = 0, playlist_counter = 0;
	char **user_dir_list, **user_playlist, user_name[30], playlist_dir[30];
	uint8_t system_status = SYSTEM_OFF, playing_mode = MODE_STOP;
	long long hb_led_millis = 0, play_led_millis = 0, gui_update_millis = 0, fps_scan_millis = 0;

	/* Initilize I/O devices and libs */
	init();

	/* Listing User Databases */
	user_dir_count = dhand.get_count();
	user_dir_list = dhand.get_listing();

	/* Setting user info to non */
	sprintf(user_name, "...");
	user_id = 0;

	/* Infinite Loop */
	while(1)
	{
		/* Checking ON/OFF switch state */
		if(buttons[0].gpio_read() == 1)
		{
			/* Lock till the switch is in hold state with debounce delay */
			while(buttons[0].gpio_read() == 1) usleep(40*1000);

			/* Toggle system state between SYSTEM_ON/SYSTEM_OFF */
			system_status = !system_status;

			/* Stop MP3 player and clear GUI if system is in OFF state */
			if(system_status == SYSTEM_OFF)
			{
				mp3.stop();
				pgui.clear_all();
			}
		}
		
		/* Check if system state is ON */
		if(system_status == SYSTEM_ON)
		{
			/* Check for Play/Pause switch state */
			if(buttons[1].gpio_read() == 1)
			{
				/* Lock till the switch is in hold state with debounce delay */
				while(buttons[1].gpio_read() == 1) usleep(40*1000);

				/* State Machine to switch between PLAY_MODE and PAUSE_MODE */
				if(playing_mode == MODE_PAUSE)
					playing_mode = MODE_PLAY;
				else if(playing_mode == MODE_PLAY)
					playing_mode = MODE_PAUSE;
			}

			/* Check for Stop switch state */
			if(buttons[2].gpio_read() == 1)
			{
				/* Lock till the switch is in hold state with debounce delay */
				while(buttons[2].gpio_read() == 1) usleep(40*1000);
				/* Set playing mode as STOP_MODE */
				playing_mode = MODE_STOP;
			}

			/* Check for playing mode and PLAY/PAUSE/STOP player accordingly */
			if(playing_mode == MODE_PAUSE && mp3.is_playing() == true)
			{
				/* Pause track */
				mp3.stop();
			}
			else if(playing_mode == MODE_PLAY && mp3.is_playing() == false && playlist_counter > 0)
			{
				/* Play track */
				mp3.play();
			}
			else if(playing_mode == MODE_STOP)
			{
				/* Pause track */
				mp3.stop();
				/* Clear GUI */
				pgui.clear_all();
				/* Reset playlist counter to stop playlist */
				playlist_counter = 0;
			}

			/* Conditional time event execution of GUI APIs which executes after every 300 mS */
			if((get_millis() - gui_update_millis) > 300 && playing_mode != MODE_STOP)
			{
				/* Update user info to GUI */
				pgui.set_user_info(user_name, user_id, 0x00ff0000);
				/* Update playlist info to GUI */
				pgui.set_playlist(user_playlist, user_playlist_count, 0x00ffff00);

				/* Update current track title info to GUI */
				if(mp3.is_playing() == true && playlist_counter > 0 && playing_mode == MODE_PLAY)
				{
					pgui.set_playing(user_playlist[playlist_counter-1], 0xffffff00);
				}
				else
				{
					pgui.set_playing("", 0);
				}

				/* Reset GUI time event millis */
				gui_update_millis = get_millis();
			}

			/* Time event execution of FPS sensor scanning API which executes after every 400 mS */
			if((get_millis() - fps_scan_millis) > 400)
			{
				/* Scanning user ID from sensor */
				fps_id = FPS.scan_id();
				/* Reset FPS sensor time event millis */
				fps_scan_millis = get_millis();
			}

			/* Check if scanned ID is valid */
			if(fps_id > 0)
			{
				/* Loop through DB */
				for(int i = 0 ; i < user_dir_count ; i++)
				{
					/* Local variables */
					int id;
					char dum;

					/* Scanning user name and ID from Ith database */
					sscanf(user_dir_list[i], "%d%c%s", &id, &dum, user_name);

					/* Compare scanned ID with user database ID */
					if(id == fps_id)
					{
						/* Check if player is in PLAY_MODE */
						if(mp3.is_playing())
							mp3.stop(); /* Stop player */

						user_id = id;
						playlist_counter = 0;
						sprintf(playlist_dir, "../../data/%s/", user_dir_list[i]);

						/* Loading user playlist directory on ID match */
						dhand.load(playlist_dir);

						/* Listing user playlist directory */
						user_playlist_count = dhand.get_count();
						user_playlist = dhand.get_listing();

						/* Changing player mode to PLAY */
						playing_mode = MODE_PLAY;

						/* Break loop on ID match */
						break;
					}
				}
			}
			
			/* Checking for playlist count */
			if(user_playlist_count > 0 && playlist_counter < user_playlist_count)
			{
				/* Check player status and mode */
				if(mp3.is_playing() == false && playing_mode == MODE_PLAY)
				{
					/* track name array */
					char track[40];

					/* Scanning track from user playlist into track array */
					sprintf(track, "%s%s", playlist_dir, user_playlist[playlist_counter]);
					/* Load track into MP3 handler object */
					mp3.load(track);
					/* Play loaded track */
					mp3.play();

					/* Increment playlist counter */
					playlist_counter++;
				}
			}
			
			/* Check if player mode is in PLAY_MODE */
			if(playing_mode == MODE_PLAY)
			{
				/* Reset RED LED */
				rgb_leds[0].gpio_write(0);

				/* Time event execution for PLAY state LED indication (Blue LED) after every 80 mS */
				if((get_millis() - play_led_millis) > 80)
				{
					/* Toggle Blue LED to indicate that track is playing */
					rgb_leds[2].gpio_write(!rgb_leds[2].gpio_read());
					/* Reset PLAY state LED time event millis */
					play_led_millis = get_millis();
				}
			}
			/* Check if player mode is in PAUSE_MODE */
			else if(playing_mode == MODE_PAUSE)
			{
				/* Reset Red LED */
				rgb_leds[0].gpio_write(0);
				/* Set Blue LED */
				rgb_leds[2].gpio_write(1);
			}
			/* Check if player mode is in STOP_MODE */
			else if(playing_mode == MODE_STOP)
			{
				/* Set Red LED */
				rgb_leds[0].gpio_write(1);
				/* Reset Blue LED */
				rgb_leds[2].gpio_write(0);
			}

			/* Time event execution of HB LED (Green LED) after every 2S */
			if((get_millis() - hb_led_millis) >= 2000)
			{
				/* Set HB Green LED */
				rgb_leds[1].gpio_write(1);
				/* Wait for 10 mS */
				usleep(10*1000);
				/* Reset HB Green LED */
				rgb_leds[1].gpio_write(0);
				/* Reset time event millis for HB LED time event */
				hb_led_millis = get_millis();
			}
		}
		/* Check if system state is OFF */
		else if(system_status == SYSTEM_OFF)
		{
			/* Reset Green & Blue LEDs */
			rgb_leds[1].gpio_write(0);
			rgb_leds[2].gpio_write(0);

			/* Time event execution of HB Green LED for OFF state indication @ 1S */
			if((get_millis() - hb_led_millis) >= 1000)
			{
				/* Toggle HB Green LED */
				rgb_leds[0].gpio_write(!rgb_leds[0].gpio_read());
				/* Reset HB LED time event millis */
				hb_led_millis = get_millis();
			}
		}

	}
	
	return(0);
}

/* void init(void) {...}
 * ...
 * @brief  : Function that initialize various I/O devices and libs.
 * @param  : None/void
 * @retval : None/void
 */
void init(void)
{
	uint8_t i;
	bool gpio_init_flag = true;

    /* Initializing mp3 handler */
    mp3.init();

	/* Initializing R305 FPS Sensor */
    std::cout << "Initializing FPS R305 Sensor ..." << std::endl;
	if(FPS.init() == false)
	{
        std::cout << "R305 FPS Sensor Error : Unable to setup R305 FPS !!" << std::endl;
        exit(1);
	}

	/* Initializing & Configure GPIOs */
	std::cout << "Initializing GPIOs ..." << std::endl;
	for(i = 0 ; i < 3 ; i++)
	{
		if(rgb_leds[i].gpio_init(RGB_LED_PINS[i], GPIO_OUT) == GPIO_ERR)
		{
			gpio_init_flag = false;
			break;
		}
		if(buttons[i].gpio_init(BUTTON_PINS[i], GPIO_IN) == GPIO_ERR)
		{
			gpio_init_flag = false;
			break;
		}
	}
	if(gpio_init_flag == false)
	{
		std::cout << "GPIO Error : Unable to configure GPIOs !!" << std::endl;
        exit(1);
	}

	/* Check for user databases */
    std::cout << "Checking User Databases ..." << std::endl;
    dhand.load("../../data/");
    if(dhand.get_count() <= 0)
    {
        std::cout << "DIR Handler Error : Database is empty !!" << std::endl;
        exit(1);
	}
}

/* long long get_millis(void) {...}
 * ...
 * @brief  : Function that returns time in milliseconds
 * @param  : None/void
 * @retval : long long (Time in mS)
 */
long long get_millis(void)
{
	/* Locals */
	struct timeval t;
	long long millis;

	/* Fetch time into struct object */
	gettimeofday(&t, NULL);
	/* Compute time in mS from actual time */
	millis = (t.tv_sec*1000LL) + (t.tv_usec/1000);
	/* Return time in mS */
	return millis;
}
