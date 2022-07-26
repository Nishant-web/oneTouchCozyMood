/*
 * Code   : Directory Handler (.cpp Source)
 * Date   : 14/04/2022 @ 02:08 AM
 */

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <stdint.h>
#include <string.h>
#include <stdlib.h>
#include "dir_handler.h"

class DIR_HANDLER;

DIR_HANDLER::DIR_HANDLER(void)
{
	dir_listing_count = 0;
	dir_listing = NULL;
 	dirent_p = NULL;
}

void DIR_HANDLER::load(const char *_abs_path)
{
	strcpy(abs_path, _abs_path);
}

uint16_t DIR_HANDLER::get_count(void)
{
	dir_listing_count = 0;

  	DIR *dir_ptr = opendir((const char*)abs_path);
	if(dir_ptr)
	{
      		while(dirent_p = readdir(dir_ptr))
		{

			if(strlen(dirent_p->d_name) > 4)
				++dir_listing_count;
		}
      		(void)closedir(dir_ptr);
	}
	return(dir_listing_count);
}

char **DIR_HANDLER::get_listing(void)
{
	uint16_t i;
	get_count();
	if(dir_listing_count > 0)
	{
		dir_listing = (char**)malloc(dir_listing_count*sizeof(char*));
		for(i = 0 ; i < dir_listing_count ; i++)
		{
			dir_listing[i] = (char*)malloc(100);
			memset(dir_listing[i], 0, 100);
		}

 	 	DIR *dir_ptr = opendir((const char*)abs_path);
		if(dir_ptr != NULL)
		{
			i = 0;
      			while((dirent_p = readdir(dir_ptr)))
			{
				if(strlen(dirent_p->d_name) > 4)
					strcpy((dir_listing[i++]), dirent_p->d_name);
			}
      			(void)closedir(dir_ptr);
		}
	}
	return(dir_listing);
}
