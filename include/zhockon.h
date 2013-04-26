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

#ifndef ZHOCKON_H
#define ZHOCKON_H
#define NULL 0
#include "base.h"
#include "platform.h"
#include <iostream>
#include <Python.h>

using std::cout;
using std::endl;
using std::string;

/*OpenGL Primitives
GL_POINTS,GL_LINES,GL_LINE_LOOP,GL_LINE_STRIP,GL_TRIANGLES,
GL_TRIANGLE_STRIP,GL_TRIANGLE_FAN,GL_QUADS,GL_QUAD_STRIP,GL_POLYGON
*/

namespace Zhockon{
struct Info{
	string name;
	string vers;
	int month;
	int year;
};

class Group;
class Geometry;
class Error{
public:
	Error();
	string getMessage();
};

class Sample{
public:
	Sample(string);
	int get();
};

class Source{ //Audio
public:
	void setSound(int);
	void setGain(float);
	void setLooping();
	void setAmbient();
	void playStream();
	void stop();
	void play();
};

class Sound{ //Audio
private:
	Source *source;
public:
	Sound(Source*);
	int loadStream(string);
	int playStream();
	void setEffects();
	Source* getSource();
};

class Audio : Package{
private:
	Source *source;
	Sound *sound;
public: 
	Audio();
};

class Surface{
public:
	Surface(int);
	void whichPoints(vector<int>);
};

class Group{ //Graphics
public:
	void addChild(Geometry*);
};

class Geometry{ //Graphics
public:
	void createPrimitiveSet(Surface*);
	void whichVertexMatrix(Matrix<int>*);
	void whichColorsMatrix(Matrix<float>*);
	void createColorBind();
	void createParent(Group*);
};

class Scene{ //Graphics
private:
        Group* root;
public:
	Scene();
	void createPyramid();
	void createCube();
	void* createObject();
	void* linkSceneObject(Geometry*);
	void* defineVertices();
	void* defineColors(Geometry*);
	void whichPrimitives(int,vector<int>,Geometry*);
	void transformMode(Geometry*);
	void drawScene();
};

class Graphics : Package{
private:
	Surface *surface;
	Geometry *geometry;
	Scene *scene;
public: 
	Graphics();
};

class Control{ //Input
public:
	Control();
};

class Input : Package{
private:
	Control *control;
public:
	Input();
};

class Element{ //Elements
private:
	int* speed;
	int width;
	int height;
	int position;
	string name;
	double hitPoints;
	Geometry* sprImage;
public:
	Element(string);
	Geometry* spriteImage();
	void hurtElement(double);
	void healElement(double);
	void whichSize(int,int);
	void whichSpeed(int,int);
	void update(Scene*);
};

class Elements : public Package{
private:
        int elemCount;
        Element* init;
        Element* last;
        Element* sprites;
public:
        Elements();
        void addElement(string);
        void remElement();
};

class View{
public:
      View();
      void setSceneData();
      void run();
};

class Player : public Package{
private:
	View* view;
public:
	Player();
};

class Python : public Binding{
private:
	PyObject* nameBind;
public:
	Python(string);
	PyObject* newObject(string,string);
	int helloPython();
	int loadModule(string);
	int pythonCall(int,char**);
};


class Physics : Package{ public: Physics(); };
class Network : Package{ public: Network(); };

Package accessPackage(string);
int newPackage(int);
int exeModule(string);
bool ZhockonInit();
void readConfig();
void createGame(string);
void deleteGame(string);
void listModules();
};
#endif