#include "physics.h"
#include "graphic.h"

b2Vec2 gravity(0.0f, -10.0f);
b2World world(gravity);

b2Body *groundBody,*body;

b2BodyDef groundBodyDef,bodyDef;
b2PolygonShape groundBox,dynamicBox;
b2FixtureDef fixtureDef;

float32 timeStep;
int32 velocityIterations,positionIterations;

void
initBOX2D(b2World* wrd)
{
	groundBodyDef.position.Set(0.0f, -10.0f);
	groundBody = world.CreateBody(&groundBodyDef);
	groundBox.SetAsBox(50.0f, 10.0f);
	groundBody->CreateFixture(&groundBox, 0.0f);

	bodyDef.type = b2_dynamicBody;
	bodyDef.position.Set(0.0f, 4.0f);
	body = world.CreateBody(&bodyDef);
	dynamicBox.SetAsBox(1.0f, 1.0f);

	fixtureDef.shape = &dynamicBox;
	fixtureDef.density = 1.0f;
	fixtureDef.friction = 0.3f;
	body->CreateFixture(&fixtureDef);

	timeStep = 1.0f / 60.0f;
	velocityIterations = 6;
	positionIterations = 2;
	
	wrd = &world;
}
