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
	private:
	uint16_t dir_listing_count;
	char abs_path[200];
	char **dir_listing;
 	struct dirent *dirent_p;

	public:
	DIR_HANDLER(void);
	void load(const char*);
	uint16_t get_count(void);
	char **get_listing(void);
};

#endif
