#include <Game/Capsule/cQuarryCapsule.h>
#include<Game/cStrategyManager.h>
namespace Game
{
namespace Capsule
{
cQuarryCapsule::cQuarryCapsule( )
{

}
cQuarryCapsule::~cQuarryCapsule( )
{

}
void cQuarryCapsule::setup(const ci::vec3 pos, const Game::Weapons::SubWeapon::SubWeaponType type)
{
	mPos = pos;
	mScale = ci::vec3(0.5f, 0.5f, 0.5f);
	mType = type;
	mAABB = ci::AxisAlignedBox(mPos - mScale / 2.f, mPos + mScale / 2.f);
}

void cQuarryCapsule::update(const float & delta_time)
{
	if (mIsget) {
		count++;
	}
	else {
		mRotate += ci::vec3(0, delta_time, 0);
	}
}
void cQuarryCapsule::draw()
{
	STRM->drawCube(mPos, mScale, mRotate, ci::ColorA(0, 0, 1, 1));
}
bool cQuarryCapsule::deleteThis()
{
	return  count > 60;
}
}
}
