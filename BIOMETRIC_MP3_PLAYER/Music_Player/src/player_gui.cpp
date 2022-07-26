/*
 * Code   : Player GUI (.cpp source)
 * Date   : 10/07/2022 @ 16:37
 */

#include "player_gui.h"

class PLAYER_GUI;

PLAYER_GUI::PLAYER_GUI(void)
{
}

PLAYER_GUI::PLAYER_GUI(uint16_t disp_w, uint16_t disp_h, uint32_t bg_color)
{
	main_win_p = new Fl_Window(disp_w, disp_h, "Biometric Music Player");
	uinfo_p = new Fl_Box(20, 25, (disp_w-40), (int)(disp_h*0.05));
	playlist_p = new Fl_Box(20, (int)(disp_h*0.05)+50, (int)(disp_w/2)-40, (int)(disp_h*0.8));
	playing_p = new Fl_Box((int)(disp_w/2)+20, (int)(disp_h*0.05)+50, (int)(disp_w/2)-40, (int)(disp_h*0.2));

	main_win_p->color(bg_color);
	main_win_p->end();
	main_win_p->show();

	uinfo_p->color(0x00202000);
	uinfo_p->labelsize((int)(disp_w*disp_h*0.00003));
	uinfo_p->box(FL_UP_BOX);

	playlist_p->color(0x00000000);
	playlist_p->labelsize((int)(disp_w*disp_h*0.000025));
	playlist_p->box(FL_UP_BOX);

	playing_p->color(0x00001000);
	playing_p->labelsize((int)(disp_w*disp_h*0.000031));
	playing_p->box(FL_UP_BOX);
	playing_p->labelfont(FL_ITALIC+FL_BOLD);
}

void PLAYER_GUI::change_bg_color(uint32_t bg_color)
{
	main_win_p->color(bg_color);
}

void PLAYER_GUI::clear_all(void)
{
	Fl::lock();
	uinfo_p->label("\0");
	playlist_p->label("\0");
	playing_p->label("\0");
	Fl::unlock();
	update();
}

void PLAYER_GUI::set_user_info(const char *name, int id, uint32_t text_color)
{
	char buff[100];

	sprintf(buff, "USER NAME: %s     USER ID: %d", name, id);

	Fl::lock();
	uinfo_p->labelcolor(text_color);
	uinfo_p->label(buff);
	Fl::unlock();
	update();
}

void PLAYER_GUI::set_playlist(char **pl_p, uint8_t pl_c, uint32_t text_color)
{
	char buff[30*pl_c] = "\0";
	uint8_t i;

	Fl::lock();
	playlist_p->labelcolor(text_color);
	for(i = 0 ; i < pl_c ; i++)
	{
		if(i == 0)
			sprintf(buff, "%d) %s\n", (i+1), *(pl_p+i));
		else
			sprintf(buff, "%s%d) %s\n", buff, (i+1), *(pl_p+i));
	}
	playlist_p->label(buff);
	Fl::unlock();
	update();
}

void PLAYER_GUI::set_playing(char *p, uint32_t text_color)
{
	char buff[100];

	sprintf(buff, "Now Playing: \n%s", p);

	Fl::lock();
	playing_p->labelcolor(text_color);
	playing_p->label(buff);
	Fl::unlock();
	update();
}

void PLAYER_GUI::update(void)
{
	Fl::wait(0.1);
}
