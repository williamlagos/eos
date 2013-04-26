/*#include <osgAL/SoundManager>
#include <osg/Group>
#include <osg/Geode>
#include <osg/Geometry>
#include <osg/PositionAttitudeTransform>
#include <osgViewer/Viewer>
#include <openalpp/Source>*/
#include <iostream>
#include <SDL/SDL.h>
#include <AL/al.h>
#include <AL/alut.h>
#include <GL/gl.h>
#include <GL/glut.h>

#define numOf(x) (sizeof(x)/sizeof*(x))

using std::cout;
using std::endl;
using std::string;

namespace Zhockon{
class SDL : public Library{
private:
	int axisx;
	int axisy;
	SDL_Event keyevent;
public:
	SDL(string,int,int);
	int baseMove();
};

class OpenAL : public Library{
private:
	ALuint Buffer;
	ALuint Source;
	ALfloat* SourcePos;
	ALfloat* SourceVel;
public:
	OpenAL(int,char**,string);
	ALboolean loadSound(char*);
};

class OpenGL : public Library{
public:
	OpenGL(string);
	void Init(int,char**,int,int);
	void loopCycle();
};
static void baseScene(void);
static void baseReshape(int,int);
};