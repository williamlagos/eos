/*
 * This file is part of Zhockon Platform project.
 * 
 * Copyright (C) 2009-2011 William Oliveira de Lagos <william.lagos1@gmail.com>
 *
 * Zhockon is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Zhockon is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Zhockon.  If not, see <http://www.gnu.org/licenses/>.
 */
#include "adapter.h"
#include <allegro.h>
using namespace Coronae;

Allegro::Allegro(string name) : Adapter(name){}

bool Allegro::helloWorld(char* word, int width, int height)
{
	if (allegro_init() != 0)
	    return 1;

	install_keyboard(); 

	if (set_gfx_mode(GFX_AUTODETECT, width, height, 0, 0) != 0) {
	    if (set_gfx_mode(GFX_SAFE, width, height, 0, 0) != 0) {
	  set_gfx_mode(GFX_TEXT, 0, 0, 0, 0);
	  allegro_message("Unable to set any graphic mode\n%s\n", allegro_error);
	  return 1;
	    }
	}

	set_palette(desktop_palette);
	clear_to_color(screen, makecol(255, 255, 255));
	acquire_screen();
	textout_centre_ex(screen, font, word, SCREEN_W/2, SCREEN_H/2, makecol(0,0,0), -1);
	release_screen();
	readkey();
}

int main(int argc, char** argv)
{
	//CoronaeInit();
        Allegro *algr = new Allegro("Algr");
	string response = algr->methodCall("base_method");
	char *sz = new char[response.length() + 1];
	strcpy(sz, response.c_str());
	if(argc <= 1)
		algr->helloWorld(sz,320,200);
	else
		algr->helloWorld(sz,atoi(argv[1]),atoi(argv[2]));
	delete [] sz;
	return 0;
}
