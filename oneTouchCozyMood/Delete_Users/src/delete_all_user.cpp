/*
 * Code   : Delete All Users
 * Date   : 10/07/2022 @ 20:20
 */

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include "fpsR305.h"
#include "dir_handler.h"

/* Class Object Declarations */
FPS_R305 FPS("/dev/serial0", 57600);   /* Finger Print Scanner Sensor Object */
DIR_HANDLER DHAND;                     /* Directory Handler Object */

int main(void)
{
	/* Main Function Local Variables */
	char **user_dir_list;
	int user_dir_count, sys_status;

	DHAND.load("../../data/");               /* Loading database directory into directory handler object  */
	user_dir_count = DHAND.get_count();      /* Getting count of sub directories / user database direcotries */
	user_dir_list = DHAND.get_listing();     /* Getting list of sub directories / user database directories */

	/* Checking WiringPi Setup */
	if(wiringPiSetup() < 0)
	{
		std::cout << "Wiring Pi Error!!\n";
		return(1);                           /* Return 1 on fail */
	}

	/* Initializing Finger Print Scanner (FPS) sensor */
	std::cout << "\nInitializing FPS R305 ";
	while(!FPS.init())
	{
		std::cout << ".";
		delay(100);
	}
	std::cout << "\nFPS R305 Initialized.\n";

	/* Printing some strings on console */
	std::cout << "\n\n";
	std::cout << "Deleting Database...\n\n";

	/* Deleting all finger print databases/templates from sensor module (R305) */
	while(!FPS.delete_all()) delay(200);

	/* Deleting all user databases from database directory (../../data/) */
	for(int i = 0 ; i < user_dir_count ; i++)
	{
		char cmd[40];

		sprintf(cmd, "rm -rf ../../data/%s", user_dir_list[i]); /* Copying formated string into cmd buffer/array */
		sys_status = system(cmd);                               /* Executing command string with system() api */
		std::cout << user_dir_list[i] << std::endl;             /* Print deleted database/directory name */
	}

	/* End of code */
	std::cout << "\nDONE!\n\n";
	return(0);
}

