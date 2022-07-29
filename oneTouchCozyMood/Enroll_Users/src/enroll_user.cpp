/*
 * Code   : Enroll User
 * Date   : 10/07/2022 @ 19:51
 */

#include <sys/types.h>
#include <sys/stat.h>
#include <iostream>
#include <stdio.h>
#include <stdint.h>
#include <wiringPi.h>
#include "fpsR305.h"

using namespace std;

/* Class object declaration */
FPS_R305 FPS("/dev/serial0", 57600);

int main(int argc, char *argv[])
{
	/* Local variables */
	char name[20], dir_name[30], dummy;
	int status;
	uint16_t id;

	/* Checking WiringPi setup */
	if(wiringPiSetup() < 0)
	{
		cout << "Wiring Pi Error!!\n";
		return(1);
	}

	/* Initialize FPS R305 Sensor */
	cout << "\nInitializing FPS R305 ";
	while(!FPS.init())
	{
		cout << ".";
		delay(100);
	}
	cout << "\nFPS R305 Initialized.\n\n\n";

	/* Printing some messgaes on console */
	cout << "Important Points:\n\n";
	cout << "* User ID range : 1-99\n";
	cout << "* User name length : 29 char Max.\n";
	cout << "* Sound track filename length : 10 char Max.\n\n";

	/* Scanning User ID & Name */
	cout << "Enter ID   : ";
	cin >> id;
	cout << "Enter Name : ";
	cin >> name;

	/* Deleting user finger print if already exists */
	while(!FPS.delete_id(id)) delay(1000);

	/* Enrolling user finger print by their ID */
	cout << "\n\nPls put your finger on sensor ...\n";
	while(!FPS.enroll_id(id)) { delay(100); }

	/* Making user database folder in DB directory */
	sprintf(dir_name, "../../data/%d-%s/", id, name);
	status = mkdir(dir_name, S_IRWXU | S_IRWXG | S_IROTH | S_IXOTH);

	/* Checking directory entry */	
	if(status == 0)
		cout << "\nThank You \"" << name << "\"(" << id << "), You are registered.\n\n";
	else
		cout << "\nError!! Unable to register user!!\n\n";

	return(0);
}

