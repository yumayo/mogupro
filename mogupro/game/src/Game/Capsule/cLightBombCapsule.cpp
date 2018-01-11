#include <Game/Capsule/cLightBombCapsule.h>
#include<Game/cStrategyManager.h>
#include<cinder/Rand.h>
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
	mLightColorH = ci::randFloat(1.f);
	float h = std::fmodf(mLightColorH, 1.0f);
	ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.0f));
	mLightSinAngle = ci::randFloat(2.f*M_PI);
	mLightRadius = mLightMaxRadius * ((1.f - mLightRate / 2.f) + mLightRate / 2.f*sin(mLightSinAngle));
	mLight = cLightManager::getInstance()->addPointLight(mPos, ci::vec3(hsv.r, hsv.g, hsv.b), mLightRadius);
}

void cLightBombCapsule::update(const float & delta_time)
{
	if (mIsget) {
		count++;
	}
	else {
		mRotate += ci::vec3(0, delta_time, 0);
		mLightColorH += delta_time;
		float h = std::fmodf(mLightColorH, 1.0f);
		ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.0f));
		mLight->color = ci::vec3(hsv.r, hsv.g, hsv.b);
		mLightSinAngle += delta_time;
		mLightRadius = mLightMaxRadius * ((1.f - mLightRate / 2.f) + mLightRate / 2.f*sin(mLightSinAngle));
		mLight->reAttachRadius(mLightRadius);
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
