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

Library::Library(string n) : Module(n,status)
{
	name = n;
	status = true;
}


SDL::SDL(string n, int x, int y) : Library(n)
{
	name = n;
	axisx = x;
	axisy = y;
}

int SDL::baseMove()
{
	while (SDL_PollEvent(&keyevent)){
		switch(keyevent.type){
			case SDL_KEYDOWN:switch(keyevent.key.keysym.sym){
						case SDLK_LEFT: axisx -= 1; break;
						case SDLK_RIGHT:axisx += 1; break;
						case SDLK_UP: 	axisy -= 1; break;
						case SDLK_DOWN: axisy += 1; break;
						default: 	break;
					}
		}
	}
	return 0;
}


OpenAL::OpenAL(int argc, char* argv[], string n) : Library(n)
{
	name = n;
	status = true;
	alutInit(&argc,argv);
	alGetError();
	for(int i = 0; i < 3; i++){
		SourcePos[i] = 0.0;
		SourceVel[i] = 0.0;
	}
}

ALboolean OpenAL::loadSound(char* filename)
{
	ALenum format;
	ALsizei size;
	ALvoid* data;
	ALsizei freq;
	ALboolean loop;
	alGenBuffers(1, &Buffer);
	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;
	alutLoadWAVFile((ALbyte*)filename, &format, &data, &size, &freq, &loop);
	alBufferData(Buffer, format, data, size, freq);
	alutUnloadWAV(format, data, size, freq);
	alGenSources(1, &Source);
	if(alGetError() != AL_NO_ERROR)
		return AL_FALSE;
	alSourcei (Source, AL_BUFFER,   Buffer   );
	alSourcef (Source, AL_PITCH,    1.0      );
	alSourcef (Source, AL_GAIN,     1.0      );
	alSourcefv(Source, AL_POSITION, SourcePos);
	alSourcefv(Source, AL_VELOCITY, SourceVel);
	alSourcei (Source, AL_LOOPING,  loop     );
	if(alGetError() == AL_NO_ERROR)
		return AL_TRUE;
	return AL_FALSE;
}


OpenGL::OpenGL(string n) : Library(n)
{
	name = n;
	status = true;
}

void OpenGL::Init(int argc,char** argv,int winHeight,int winWidth)
{
	glutInit(&argc,argv);
	glutInitDisplayMode(GLUT_DEPTH | GLUT_DOUBLE | GLUT_RGBA);
	glutInitWindowPosition(100,100);
	glutInitWindowSize(winHeight,winWidth);
	glutCreateWindow("Zhockon");
}

void Zhockon::baseScene(void)
{
	float angle = 0.0;
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
	glLoadIdentity();
	gluLookAt(0.0, 0.0, 10.0,
		  0.0, 0.0,  0.0,
		  0.0, 1.0,  0.0);
	glRotatef(angle,0.0,1.0,0.0);
	glBegin(GL_POLYGON);
		glVertex3f(-2.0,-2.0,0.0);
		glVertex3f(2.0,0.0,0.0);
		glVertex3f(0.0,2.0,0.0);
		glVertex3f(-1.0,0.0,0.0);
	glEnd();
	angle += 0.1;
	glutSwapBuffers();
}

void Zhockon::baseReshape(int width, int height)
{
	if(height == 0)
		height == 1;
	float ratio = width*1.0/height;
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glViewport(0,0,width,height);
	gluPerspective(45,ratio,1,1000);
	glMatrixMode(GL_MODELVIEW);
}

void OpenGL::loopCycle()
{
	glutDisplayFunc(baseScene);
	glutReshapeFunc(baseReshape);
	glutIdleFunc(baseScene);
	glutMainLoop();
}
