#include <Game/Capsule/cLightBombCapsule.h>
#include<Game/cStrategyManager.h>
#include<cinder/Rand.h>
#include"Resource\cObjectManager.h"
#include"Resource\cImageManager.h"
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
	mScale = ci::vec3(1.f);
	mType = type;
	mAABB = ci::AxisAlignedBox(mPos - mScale / 2.f, mPos + mScale / 2.f);
	mLightColorH = ci::randFloat(1.f);
	float h = std::fmodf(mLightColorH, 1.0f);
	ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.0f));
	mLightSinAngle = ci::randFloat(2.f*M_PI);
	mLightRadius = mLightMaxRadius * (((1.f - mLightRate) / 2.f) + mLightRate / 2.f*sin(mLightSinAngle));
	mLight = cLightManager::getInstance()->addPointLight(mPos, ci::vec3(hsv.r, hsv.g, hsv.b), mLightRadius);
	mesh = Resource::cObjectManager::getInstance()->findObject("capsule.obj");
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
		mLightRadius = mLightMaxRadius * (((1.f - mLightRate) / 2.f) + mLightRate / 2.f*sin(mLightSinAngle));
		mLight->reAttachRadius(mLightRadius);
	}
	
}
void cLightBombCapsule::draw()
{
	ci::gl::ScopedTextureBind tex(Resource::IMAGE["in_game/capsule.png"]);
	ci::gl::pushModelView();
	ci::gl::translate(mPos + ci::vec3(0, 0, 0));
	ci::gl::rotate(mRotate.y, ci::vec3(1, 0, 0));
	ci::gl::rotate(mRotate.y, ci::vec3(0, 1, 0));
	ci::gl::translate(ci::vec3(0, -mScale.y/2.f, 0));
	ci::gl::color(ci::ColorA(1,0,0,1));
	ci::gl::scale(mScale*(1.f / 64.f));
	ci::gl::draw(mesh);
	ci::gl::popModelView();
	//STRM->drawCube(mPos, mScale, mRotate, ci::ColorA(1, 0, 0, 1));
}
bool cLightBombCapsule::deleteThis()
{
	return count > 30;
}
}
}
