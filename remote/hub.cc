/*
 * This file is part of Efforia Open Source Initiative.
 * 
 * Copyright (C) 2009-2015 William Oliveira de Lagos <william@efforia.com.br>
 *
 * Efforia OSI is free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 *
 * Efforia OSI is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with Efforia OSI.  If not, see <http://www.gnu.org/licenses/>.
 */

#include <hub.h>

App::App()
{

}

int App::start()
{
	std::cout << "HUB command-line utility" << std::endl;
	return 0;
}

int main(int argc, char** argv)
{
	App *app = new App();
	app->start();
}
