#include <Box2D/Box2D.h>

extern b2Body *groundBody,*body;

extern b2Vec2 gravity;
extern b2World world;
extern b2BodyDef groundBodyDef,bodyDef;
extern b2PolygonShape groundBox,dynamicBox;
extern b2FixtureDef fixtureDef;

extern float32 timeStep;
extern int32 velocityIterations,positionIterations;

void initBOX2D(b2World *wrd);
