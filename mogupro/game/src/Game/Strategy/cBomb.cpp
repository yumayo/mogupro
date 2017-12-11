#include <Game/Strategy/cBomb.h>
#include"Game/cStrategyManager.h"
using namespace ci;
using namespace ci::app;

using namespace Node;

using namespace Node::Action;

namespace Game
{
namespace Strategy
{

cBomb::cBomb(const ci::vec3 _pos, const ci::vec3 _speed, const ci::vec3 _scale, const bool _ismyobject)
	: aabb(_pos,_scale)
	, rb(aabb)
{
	speed = _speed;
	mPos = _pos;
	mPos += speed*0.3f + vec3(0, 1, 0);
	aabb.setPosition(mPos);
	speed = glm::normalize(speed);
	speed.y += 0.1f;
	scale = _scale;
	ismyobject = _ismyobject;
	/*speed *= 7.f;
	
	speed.y += 4.f;*/
	vec4 zero(1);
	vec3 y_zero = vec3(speed.x, 0.0f, speed.z);
	
	y_zero = glm::normalize(y_zero);
	vec3 y_zerobuf = y_zero;

	vec3 y_zero_buf2 = glm::normalize(y_zerobuf +vec3(0,1,0));

	vec3 c = glm::cross(y_zero_buf2, y_zero);

	//float r = randInt(32);
	glm::quat q = glm::quat(M_PI / 4.f, c);

	float angle = atan2f(c.y, c.z);
	//
	mat4 m = glm::toMat4(q);

	zero = zero * m;
	speed = vec3(zero);
	
}
cBomb::~cBomb( )
{
	aabb.removeWorld();
	rb.removeWorld();
}
void cBomb::draw()
{
	STRM->drawCube(mPos, scale, vec3(0, 0, 0), ColorA(1, 0, 0, 1));
}
void cBomb::update(const float & delta_time)
{
	//speed.y -= delta_time*1.f;
	//mPos += speed*delta_time;
	/*rb.setSpeed(speed);
	mPos = aabb.getPosition();*/
}
void cBomb::updateCollisionAfterUpdate(const float & delta_time)
{
	mPos = aabb.getPosition();
	speed = rb.getSpeed();
}
void cBomb::setup()
{
	aabb.addWorld();
	rb.addWorld();
	rb.setSpeed(speed);
}
bool cBomb::DeleteThis()
{
	return false;
}
void cBomb::setField(const ci::vec3 pos)
{
}
}
}
