/*
 * Code   : Enroll User
 * Date   : 03/04/2022 @ 19:55
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <wiringPi.h>
#include <wiringSerial.h>
#include "fpsR305.h"

FPS_R305 FPS("/dev/serial0", 57600);

int main(void)
{
	char name[50], dir_name[100], dummy;
	int status;
	uint16_t id;

	if(wiringPiSetup() < 0)
	{
		printf("Wiring Pi Error!!\n");
		return(1);
	}

	printf("\nInitializing FPS R305 ");
	while(!FPS.init())
	{
		printf(".");
		delay(100);
	}
	printf("\nFPS R305 Initialized.\n");

	printf("\n\n");
	printf("Enter ID   : ");
	scanf("%hd", &id);
	printf("Enter Name : ");
	scanf("%c %[^\n]s", &dummy, name);

	while(!FPS.delete_id(id)) delay(1000);

	printf("\n\nPls put your finger on sensor ...\n");
	while(!FPS.enroll_id(id)) { delay(100); }

	sprintf(dir_name, "../../data/%d-%s/", id, name);
	status = mkdir(dir_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);
	
	if(status == 0)
		printf("\nThank You \"%s\" (%d), You are registered.\n\n", name, id);	
	else
		printf("\nError!! Unable to register user!!\n\n");

	return(0);
}

