/*
 * This file is part of Zhockon Platform project.
 * 
 * Copyright (C) 2009-2011 William Oliveira de Lagos <williamx64@gmail.com>
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

#include "zkn.h"
#include "euphoria.h"

PyGame::PyGame()
{
	/* Option not implemented already */
}

Euphoria::Euphoria(bool stat) : Module(name,status)
{
	name = "Euphoria";
	status = stat;
}
int Euphoria::startEngine()
{
	/* Option not implemented already */
}
int Euphoria::stopEngine()
{
	/* Option not implemented already */
}

Naanphea::Naanphea(bool stat) : Module(name,status)
{
	name = "Naanphea";
	status = stat;
}
int Naanphea::startEngine()
{
	game.startEngine();
}
int Naanphea::stopEngine()
{
	game.stopEngine();
}

ZKN::ZKN(bool naanStatus=false,bool euphStatus=false)
{
	naanCore = new Naanphea(naanStatus);
	euphCore = new Euphoria(euphStatus);
	gameClasses.insert(pair<void*,bool>((void*)naanCore,naanCore->getStatus()));
	gameClasses.insert(pair<void*,bool>((void*)euphCore,euphCore->getStatus()));
}
int ZKN::chooseGameClass(int gameType)
{
	switch(gameType){
	  case 2: gameClasses[(void*)euphCore] = true;
		  engineHandler = (void*)euphCore;
		  break;
	  case 3: gameClasses[(void*)naanCore] = true;
		  engineHandler = (void*)naanCore;
		  break;
	}
}
int ZKN::callZKNModule()
{
	/* Option not implemented already */
}
void* ZKN::getEngineHandler()
{
	return engineHandler;
}
void* ZKN::getMicroModule()
{

}