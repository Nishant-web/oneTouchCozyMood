/*
 * Code   : Player GUI (.h Header)
 * Date   : 10/07/2022 @ 16:32
 */

#ifndef __PLAYER_GUI_H__
#define __PLAYER_GUI_H__

#include <FL/Fl.H>
#include <FL/Fl_Window.H>
#include <FL/Fl_Box.H>
#include <stdio.h>
#include <unistd.h>
#include <stdint.h>

class PLAYER_GUI
{
	private:
	Fl_Window *main_win_p;
	Fl_Box *uinfo_p, *playlist_p, *playing_p;

	public:
	PLAYER_GUI(void);
	PLAYER_GUI(uint16_t, uint16_t, uint32_t);
	void change_bg_color(uint32_t);
	void clear_all(void);
	void set_user_info(const char*, int, uint32_t);
	void set_playlist(char**, uint8_t, uint32_t);
	void set_playing(char*, uint32_t);
	void update(void);

};

#endif
