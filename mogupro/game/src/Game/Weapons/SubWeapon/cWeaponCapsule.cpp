#include <Game/Weapons/SubWeapon/cWeaponCapsule.h>
#include"Resource\cObjectManager.h"
#include"Game\cPlayerManager.h"
#include "Game/cSubWeaponManager.h"
#include <Game/cClientAdapter.h>
#include <Utility/cInput.h>
#include"Sound/Wav.h"
#include"Sound\Stereophonic.h"
#include<Particle/cParticleManager.h>
namespace Game
{
namespace Weapons
{
namespace SubWeapon
{

cWeaponCapsule::cWeaponCapsule(const ci::vec3 pos, const ci::vec3 speed, const int playerid, const int objectid, const Game::Weapons::SubWeapon::SubWeaponType type)
	: aabb(pos, mScale)
	, rb(aabb) {
	mPos = pos;
	mDrawScale = mScale / 2.f;
	mObjectId = objectid;
	mSpeed = speed;
	mPlayerId = playerid;
	mTeamNum = cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getWhichTeam();
	mIsMyObject = (cPlayerManager::getInstance()->getActivePlayer()->getPlayerId() == mPlayerId);
	mType = type;
}
cWeaponCapsule::~cWeaponCapsule( )
{
	aabb.removeWorld();
	rb.removeWorld();
}
void cWeaponCapsule::setup()
{
	aabb.addWorld();
	rb.addWorld();
	rb.setSpeed(mSpeed);
	rb.setFriction(1.0f);
	aabb.setSize(mScale);
	mesh = Resource::cObjectManager::getInstance()->findObject("sphere.obj");
	Sound::StereophonicManager::getInstance()->add("lightbombthrow" + std::to_string(mObjectId),ci::app::getAssetPath("SE/SubWeapon/lightbombthrow.wav").string(), mPos);
}
void cWeaponCapsule::update(const float & delta_time)
{
	
}
void cWeaponCapsule::updateCollisionAfterUpdate(const float & delta_time)
{
	
	mPos = aabb.getPosition();
	mSpeed = rb.getSpeed();
	HitLand();
	
}
void cWeaponCapsule::draw()
{
	ci::gl::pushModelView();
	ci::gl::translate(mPos);
	ci::gl::color(ci::ColorA(1,1,1));
	ci::gl::scale(mDrawScale);
	ci::gl::draw(mesh);
	ci::gl::popModelView();
}
bool cWeaponCapsule::deleteThis()
{
	return isDelete;
}
int cWeaponCapsule::getPlayerId()
{
	return mPlayerId;
}
int cWeaponCapsule::getObjectId()
{
	return mObjectId;
}
ci::vec3 cWeaponCapsule::getPos()
{
	return mPos;
}
ci::vec3 cWeaponCapsule::getScale()
{
	return mScale;
}
void cWeaponCapsule::HitLand()
{
	if (mIsHitLand)return;
	if (rb.isLanding()) {
		mIsHitLand = true;
		isDelete = true;
		Sound::StereophonicManager::getInstance()->add("capselopen" + std::to_string(mObjectId), ci::app::getAssetPath("SE/SubWeapon/capselopen.wav").string(), mPos);
		
		Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
			.scale(1.5f).
			vanishTime(0.5f).
			vanishTimeRange(0.0f).
			speed(0.30f).
			addVec(ci::vec3(0, 0.5, 0)).
			textureType(Particle::ParticleTextureType::SMOKE).
			color(ci::ColorA::white()).
			moveType(Particle::ParticleType::EXPROTION).count(30).isTrajectory(true).gravity(0.24));

		if (mIsMyObject) {
			createSubWeapon();

		/*	Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
				.scale(1).
				vanishTime(0.7f).effectTime(1).
				speed(0).
				textureType(Particle::ParticleTextureType::SMOKE).
				color(ci::ColorA::white()).
				addVec(ci::vec3(0,0.05f,0)).
				moveType(Particle::ParticleType::GLITTER).count(30).isTrajectory(false).gravity(0));*/
			//Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
			//	.scale(1.5f).
			//	vanishTime(0.7f).
			//	vanishTimeRange(0.0f).
			//	speed(0.30f).
			//	addVec(ci::vec3(0,-0.25,0)).
			//	textureType(Particle::ParticleTextureType::SMOKE).
			//	color(ci::ColorA::white()).
			//	moveType(Particle::ParticleType::EXPROTION).count(30).isTrajectory(true).gravity(-0.12));
			
		}
	}
}
void cWeaponCapsule::createSubWeapon()
{
	float length = 2.5f;
	switch (mType)
	{
	case Game::Weapons::SubWeapon::SubWeaponType::LIGHT_BOMB:
		Game::cClientAdapter::getInstance()->sendLightBomb(mPos, mSpeed);
		break;
	case Game::Weapons::SubWeapon::SubWeaponType::QUARRY:
		Game::cClientAdapter::getInstance()->sendSetQuarry(mPos + ci::vec3(length*cos(2.f*M_PI*(0.f/3.f)), 1.5, length*sin(2.f*M_PI*(0.f / 3.f))));
		Game::cClientAdapter::getInstance()->sendSetQuarry(mPos + ci::vec3(length*cos(2.f*M_PI*(1.f / 3.f)), 1.5, length*sin(2.f*M_PI*(1.f / 3.f))));
		Game::cClientAdapter::getInstance()->sendSetQuarry(mPos + ci::vec3(length*cos(2.f*M_PI*(2.f / 3.f)), 1.5, length*sin(2.f*M_PI*(2.f / 3.f))));
		break;
	default:
		break;
	}
}
}
}
}
