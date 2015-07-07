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

#include <zhockon.h>
using namespace Zhockon;

Element::Element(string n)
{
	name = n;
	sprImage = spriteImage();
}

Geometry* Element::spriteImage()
{
	Geometry* sprImg = new Geometry();
	return sprImg;
}
void Element::hurtElement(double hurtPoints)
{
	hitPoints -= hurtPoints;
}
void Element::healElement(double healPoints)
{
	hitPoints += healPoints;
}
void Element::whichSize(int newWidth, int newHeight)
{
	width = newWidth;
	height = newHeight;
}
void Element::whichSpeed(int xAxis, int yAxis)
{
	speed = &xAxis;
	speed++;
	speed = &yAxis;
}
void Element::update(Scene* sce)
{
	sce->drawScene();
}


Elements::Elements()
{
	Scene *sce = new Scene();
	elemCount = 0;
}

void Elements::addElement(string n)
{
	if(sprites){
		sprites++;
		sprites = new Element(n);
		last = sprites;
	}else{
		init = sprites;
		sprites = new Element(n);
	}
	elemCount++;
}
void Elements::remElement()
{
	Element* removed;
        if(sprites){
		removed = last;
		delete removed;
		last--; elemCount--;
	}
}