/*
 * Code   : Enroll User
 * Date   : 10/07/2022 @ 20:20
 */

#include <iostream>
#include <unistd.h>
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include "fpsR305.h"
#include "dir_handler.h"

FPS_R305 FPS("/dev/serial0", 57600);
DIR_HANDLER DHAND;

int main(void)
{
	char **user_dir_list;
	int user_dir_count, sys_status;

	DHAND.load("../../data/");
	user_dir_count = DHAND.get_count();
	user_dir_list = DHAND.get_listing();

	if(wiringPiSetup() < 0)
	{
		std::cout << "Wiring Pi Error!!\n";
		return(1);
	}

	std::cout << "\nInitializing FPS R305 ";
	while(!FPS.init())
	{
		std::cout << ".";
		delay(100);
	}
	std::cout << "\nFPS R305 Initialized.\n";

	std::cout << "\n\n";
	std::cout << "Deleting Database...\n\n";

	while(!FPS.delete_all()) delay(200);

	for(int i = 0 ; i < user_dir_count ; i++)
	{
		char cmd[40];

		sprintf(cmd, "rm -rf ../../data/%s", user_dir_list[i]);
		sys_status = system(cmd);
		std::cout << user_dir_list[i] << std::endl;
	}
	
	std::cout << "\nDONE!\n\n";

	return(0);
}

