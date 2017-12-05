#include <Game/Capsule/cLightBombCapsule.h>
#include<Game/cStrategyManager.h>
namespace Game
{
namespace Capsule
{
cLightBombCapsule::cLightBombCapsule( )
{

}
cLightBombCapsule::~cLightBombCapsule( )
{

}
void cLightBombCapsule::setup(const ci::vec3 pos, const Game::Weapons::SubWeapon::SubWeaponType type)
{
	mPos = pos;
	mScale = ci::vec3(0.5f, 0.5f, 0.5f);
	mType = type;
	mAABB = ci::AxisAlignedBox(mPos - mScale / 2.f, mPos + mScale / 2.f);
	mLight = cLightManager::getInstance()->addPointLight(mPos, ci::vec3(0, 1, 0), 1.f);
}

void cLightBombCapsule::update(const float & delta_time)
{
	if (mIsget) {
		count++;
	}
	else {
		mRotate += ci::vec3(0, delta_time, 0);
	}
	
}
void cLightBombCapsule::draw()
{
	STRM->drawCube(mPos, mScale, mRotate, ci::ColorA(1, 0, 0, 1));
}
bool cLightBombCapsule::deleteThis()
{
	return count > 60;
}
}
}
