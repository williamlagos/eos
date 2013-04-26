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

#include "zhockon/zhockon.h"
#include "naanphea/naanphea.h"
#include "euphoria/euphoria.h"
#include <utility>
#include <map>
#include <iostream>
#include <Python.h>

using std::cout;
using std::endl;
using std::string;
using std::map;
using std::pair;

class PyCore{
public:
	PyCore();
	void showInfo();
	void confCore();
	void testLibs();
};

class PyGame{
	PyObject* sprites;
	PyObject* video;
	PyObject* gamespace;
	int width;
	int height;
public:
	PyGame();
	void startSync();
	void stopSync();
	void reconfigureVideo();
	void addSprite();
	void removeSprite();
	void drawImage();
	void writeText();
	void drawBackground();
	void drawSprites();
	void update();
};

class Euphoria : public Module{
private:
	Core core;
public:
	Euphoria(bool);
	int startEngine();
	int stopEngine();
};

class Naanphea : public Module{
private:
	Game game;
public:
	Naanphea(bool);
	int startEngine();
	int stopEngine();
};

class ZKN{
private:
	map<void*,bool> gameClasses;
	map<void*,bool>::iterator it;
	Naanphea* naanCore;
	Euphoria* euphCore;
	void* engineHandler;
public:
	ZKN(bool,bool);
	int chooseGameClass(int);
	int callZKNModule();
	void* getEngineHandler();
	void* getMicroModule();
};