/*
 * Code   : Directory Handler (.h Header)
 * Date   : 14/04/2022 @ 01:50 AM
 */

#ifndef __DIR_HANDLER_H
#define __DIR_HANDLER_H

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>

class DIR_HANDLER
{
	/* Private data members for Directory Handler (DIR_HANDLER) class*/
	private:
	uint16_t dir_listing_count;
	char abs_path[100];
	char **dir_listing;
 	struct dirent *dirent_p;

	/* Public members for Directory Handler (DIR_HANDLER) class*/
	public:

	/* Constructor & member function declarations */
	DIR_HANDLER(void);
	void load(const char*);
	uint16_t get_count(void);
	char **get_listing(void);
};

#endif
