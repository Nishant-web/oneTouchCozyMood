/*
 * Code   : Biometric Music Player
 * Date   : 03/04/2022 @ 19:55
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <wiringPi.h>
#include <wiringSerial.h>

#include "fpsR305.h"
#include "mp3_handler.h"
#include "dir_handler.h"

#define LED_R   28
#define LED_B   29

FPS_R305 FPS("/dev/serial0", 57600);

MP3 mp3;
DIR_HANDLER DHAND;

int id, _id;
uint16_t db_dir_count = 0, playlist_count = 0, i;
char *str, **db_dir_list, **playlist, playlist_path[110], name[100], c;

uint8_t led_counter = 0;
unsigned long led_millis = 0;

int main(void)
{
	if(wiringPiSetup() < 0)
	{
		printf("Wiring Pi Error!!\n");
		return(1);
	}

	pinMode(LED_R, OUTPUT);
	pinMode(LED_B, OUTPUT);

	printf("\nInitializing FPS R305 ");
	while(!FPS.init())
	{
		printf(".");
		delay(100);
	}
	printf("\nFPS R305 Initialized.\n");
	
	mp3.init();

	DHAND.load("../../data/");
	db_dir_count = DHAND.get_count();
	db_dir_list  = DHAND.get_listing();

	while(1)
	{
		id = FPS.scan_id();
		if(id > 0)
		{
			for(i = 0 ; i < db_dir_count ; i++)
			{
				
				if(strlen(db_dir_list[i]) > 1)
				{
					str = db_dir_list[i];
					sscanf(str, "%d%c%s", &_id, &c, name);
					if(_id == id)
						break;
				}
			}

			if(i < db_dir_count)
			{
				printf("\n\n>>>>>>>> Wel-Come <<<<<<<<\n\n");
				printf("Name : %s\n", name);
				printf("ID   : %d\n", id);

				bzero(playlist_path, sizeof(playlist_path));

				strcpy(playlist_path, "../../data/");
				strcat(playlist_path, db_dir_list[i]);

				DHAND.load(playlist_path);
				playlist_count = DHAND.get_count();
				playlist = DHAND.get_listing();
				
				if(playlist_count > 0)
				{
					printf("\nYour Playlist :\n\n");
					for(i = 0 ; i < playlist_count ; i++)
						if(strlen(playlist[i]) > 2)
							printf("%s\n", playlist[i]);
					printf("\n\n");
				}
			}
			i = 0;
		}

		if(playlist_count > 0 && mp3.is_playing() == false)
		{
			if(strlen(playlist[i]) > 2)
			{
				bzero(playlist_path, sizeof(playlist_path));
				strcpy(playlist_path, "../../data/");
				strcat(playlist_path, str);
				strcat(playlist_path, "/");
				strcat(playlist_path, playlist[i]);

				digitalWrite(LED_R, LOW);
				digitalWrite(LED_B, HIGH);

				while(mp3.is_playing());
				mp3.stop();
				while(mp3.is_playing());
				mp3.load(playlist_path);
				mp3.play();
				while(!mp3.is_playing());
				printf("Now Playing : %s\n", playlist[i]);
			}
			playlist_count--;
			++i;
		}

		if(mp3.is_playing())
		{
			if((millis() - led_millis) > 180)
			{
				switch(led_counter)
				{
					case 0:
					digitalWrite(LED_R, HIGH);
					digitalWrite(LED_B, LOW);
					break;

					case 1:
					digitalWrite(LED_R, LOW);
					digitalWrite(LED_B, HIGH);
					break;

					case 2:
					digitalWrite(LED_R, HIGH);
					digitalWrite(LED_B, HIGH);
					break;
				}

				led_counter++;
				if(led_counter > 2)
					led_counter = 0;

				led_millis = millis();
			}
		}
		else
		{
			digitalWrite(LED_R, HIGH);
			digitalWrite(LED_B, LOW);
		}
	}
	return(0);
}
