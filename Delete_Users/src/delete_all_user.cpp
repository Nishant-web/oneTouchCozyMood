/*
 * Code   : Delete User
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
	printf("Deleting Database...\n");

	while(!FPS.delete_all()) delay(200);
	
	printf("\nDONE!\n\n");

	return(0);
}

