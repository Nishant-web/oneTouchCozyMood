/*
 * Code   : Player GUI (.cpp source)
 * Date   : 10/07/2022 @ 16:37
 */

#include "player_gui.h"

/* Class Declaration */
class PLAYER_GUI;

/* PLAYER_GUI::PLAYER_GUI(void) {...}
 * ...
 * @brief  : Constructor member of class PLAYER_GUI.
 * @param  : None/void
 * @retval : None/void
 */
PLAYER_GUI::PLAYER_GUI(void)
{
}

/* PLAYER_GUI::PLAYER_GUI(uint16_t, uint16_t, uint32_t) {...}
 * ...
 * @brief  : Constructor member of class PLAYER_GUI to initialize data members, & GUI.
 * @param  : uint16_t (GUI Display Width), uint16_t (GUI Display Height), uint32_t (GUI BG color code)
 * @retval : None/void
 */
PLAYER_GUI::PLAYER_GUI(uint16_t disp_w, uint16_t disp_h, uint32_t bg_color)
{
	/* Initilize FLTK GUI objects */
	main_win_p = new Fl_Window(disp_w, disp_h, "Biometric Music Player");
	uinfo_p = new Fl_Box(20, 25, (disp_w-40), (int)(disp_h*0.05));
	playlist_p = new Fl_Box(20, (int)(disp_h*0.05)+50, (int)(disp_w/2)-40, (int)(disp_h*0.8));
	playing_p = new Fl_Box((int)(disp_w/2)+20, (int)(disp_h*0.05)+50, (int)(disp_w/2)-40, (int)(disp_h*0.2));

	/* Setting main GUI window parameters */
	main_win_p->color(bg_color);
	main_win_p->end();
	main_win_p->show();

	/* Setting user interface GUI block parameters */
	uinfo_p->color(0x00202000);
	uinfo_p->labelsize((int)(disp_w*disp_h*0.00003));
	uinfo_p->box(FL_UP_BOX);

	/* Setting playlist GUI block parameters */
	playlist_p->color(0x00000000);
	playlist_p->labelsize((int)(disp_w*disp_h*0.000025));
	playlist_p->box(FL_UP_BOX);

	/* Setting current playing track GUI block parameters */
	playing_p->color(0x00001000);
	playing_p->labelsize((int)(disp_w*disp_h*0.000031));
	playing_p->box(FL_UP_BOX);
	playing_p->labelfont(FL_ITALIC+FL_BOLD);
}

/* void PLAYER_GUI::change_bg_color(uint32_t) {...}
 * ...
 * @brief  : Member function of class PLAYER_GUI to set background color of main window.
 * @param  : uint32_t (GUI BG color code)
 * @retval : None/void
 */
void PLAYER_GUI::change_bg_color(uint32_t bg_color)
{
	/* Setting color parameter of main window */
	main_win_p->color(bg_color);
}

/* void PLAYER_GUI::clear_all(void) {...}
 * ...
 * @brief  : Member function of class PLAYER_GUI to clear all text boxes.
 * @param  : None/void
 * @retval : None/void
 */
void PLAYER_GUI::clear_all(void)
{
	Fl::lock();
	uinfo_p->label("\0");
	playlist_p->label("\0");
	playing_p->label("\0");
	Fl::unlock();
	update();
}

/* void PLAYER_GUI::set_user_info(const char*, int, uint32_t) {...}
 * ...
 * @brief  : Member function of class PLAYER_GUI to set user info text with specific color.
 * @param  : const char* (Pointer to name string of user), int (User ID), uint32_t (User info text color code)
 * @retval : None/void
 */
void PLAYER_GUI::set_user_info(const char *name, int id, uint32_t text_color)
{
	/* User info text buffer/array */
	char buff[100];

	/* Copying user info formatted string into buffer/array */
	sprintf(buff, "USER NAME: %s     USER ID: %d", name, id);

	/* Locking FLTK object before changing parameters */
	Fl::lock();
	/* Setting text color code */
	uinfo_p->labelcolor(text_color);
	/* Setting text */
	uinfo_p->label(buff);
	/* Un-locking FLTK object after changing parameters */
	Fl::unlock();
	/* Update GUI */
	update();
}

/* void PLAYER_GUI::set_playlist(char**, uint8_t, uint32_t) {...}
 * ...
 * @brief  : Member function of class PLAYER_GUI to set user playlist with specific color.
 * @param  : char** (Double pointer to playlist array), uint8_t (Playlist count), uint32_t (Playlist text color code)
 * @retval : None/void
 */
void PLAYER_GUI::set_playlist(char **pl_p, uint8_t pl_c, uint32_t text_color)
{
	/* Local varibles */
	char buff[30*pl_c] = "\0";
	uint8_t i;

	/* Locking FLTK object before changing parameters */
	Fl::lock();
	/* Setting playlist text color */
	playlist_p->labelcolor(text_color);
	for(i = 0 ; i < pl_c ; i++)
	{
		if(i == 0)
			sprintf(buff, "%d) %s\n", (i+1), *(pl_p+i));
		else
			sprintf(buff, "%s%d) %s\n", buff, (i+1), *(pl_p+i));
	}
	playlist_p->label(buff);
	/* Un-locking FLTK object before changing parameters */
	Fl::unlock();
	/* Update GUI */
	update();
}

/* void PLAYER_GUI::set_playing(char*, uint32_t) {...}
 * ...
 * @brief  : Member function of class PLAYER_GUI to set current playing track title with specific color.
 * @param  : char* (Pointer to current/now-plyaing track name), uint32_t (Playlist text color code)
 * @retval : None/void
 */
void PLAYER_GUI::set_playing(char *p, uint32_t text_color)
{
	/* Current playing track name buffer/array  */
	char buff[100];

	/* Copying formatted string to buff */
	sprintf(buff, "Now Playing: \n%s", p);

	/* Locking FLTK object before changing parameters */
	Fl::lock();
	playing_p->labelcolor(text_color);
	playing_p->label(buff);
	/* Un-locking FLTK object after changing parameters */
	Fl::unlock();
	/* Update GUI */
	update();
}

/* void PLAYER_GUI::update(void) {...}
 * ...
 * @brief  : Member function of class PLAYER_GUI to update GUI.
 * @param  : None/void
 * @retval : None/void
 */
void PLAYER_GUI::update(void)
{
	/* Updating GUI API call */
	Fl::wait(0.1);
}
