#include <Game/Weapons/UseSubWeapon/cUseWeaponCapsel.h>
#include<Game/cSubWeaponManager.h>
#include <Utility/cInput.h>
#include<Game/cPlayerManager.h>
#include <Game/cClientAdapter.h>
#include"Resource\cSoundManager.h"
#include"cinder\gl\gl.h"
#include"Game/cPlayerManager.h"
#include"Collision\cCollisionManager.h"
#include"Game\cStrategyManager.h"
namespace Game
{
namespace Weapons
{
namespace UseSubWeapon
{
cUseWeaponCapsel::cUseWeaponCapsel( )
	: collider(cPlayerManager::getInstance()->getActivePlayer()->getPos(), ci::vec3(0.5f))
	, rigid(collider) {

}
cUseWeaponCapsel::~cUseWeaponCapsel( )
{
	collider.removeWorld();
	rigid.removeWorld();
}
void cUseWeaponCapsel::setup(const int playerid, const Game::Weapons::SubWeapon::SubWeaponType type)
{
	mPlayerId = playerid;
	mType = type;
	Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getWhichTeam();
	collider.addWorld();
	rigid.addWorld();
	collider.setLayer(1);

	light = cLightManager::getInstance()->addPointLight(Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPos(),
		ci::vec3(1, 1, 1), 1.5f);

	setSpeed();
}

void cUseWeaponCapsel::update(const float & delta_time)
{
	
}
void cUseWeaponCapsel::updateCollisionAfterUpdate(const float & delta_time)
{
	if (mIsdelete)return;

	deltaColor += delta_time;


	setSpeed();
	collider.setPosition(beginpos);
	rigid.setSpeed(speed);

	createLootPos();
	if (!mLootPos.empty()) {
		light->reAttachPosition(mLootPos.back());
	}


	sincolor += delta_time;

	float h = std::fmodf(sincolor, 1.0f);

	ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.f));
	light->color = hsv;

	if (!ENV->pressKey(ci::app::KeyEvent::KEY_g)) {
		createSubWeapon();
		mIsdelete = true;
		return;
	}
}
void cUseWeaponCapsel::draw()
{
	if (mIsdelete)return;

	for (int i = 0; i < mLootPos.size() - 1; ++i)
	{
		ci::gl::ScopedLineWidth width(10.f);
		float h = mColorT[mLootPos.size() - 1 - i];
		ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.f));
		ci::gl::ScopedColor col(ci::ColorA(hsv.r, hsv.g, hsv.b, 1));
		ci::gl::drawLine(mLootPos[i], mLootPos[i + 1]);
	}
}
bool cUseWeaponCapsel::deleteThis()
{
	return mIsdelete;
}
void cUseWeaponCapsel::createSubWeapon()
{
	ci::vec3 playerDir = cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getPlayerVec();
	playerDir = glm::normalize(playerDir);

	ci::vec3 pos = cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getPos();
	ci::vec3 tangent = glm::rotateY(playerDir, glm::pi<float>() * 0.5F);

	pos += ci::vec3(-tangent.x, tangent.y, -tangent.z) / 4.f;

	ci::vec3 normalizedSpeed = playerDir * glm::angleAxis(glm::radians(50.0F), tangent);

	float power = 10.0F;

	Game::cSubWeaponManager::getInstance()->createWeaponCapsel(pos, normalizedSpeed*power,
		mPlayerId, Game::cSubWeaponManager::getInstance()->debugcapselcount, mType);


	//cClientAdapter::getInstance()->sendLightBomb(pos, normalizedSpeed * power);
}
int cUseWeaponCapsel::getPlayerId()
{
	return mPlayerId;
}
void cUseWeaponCapsel::setSpeed()
{
	ci::vec3 playerDir = cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getPlayerVec();
	playerDir = glm::normalize(playerDir);
	ci::vec3 pos = cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getPos();
	ci::vec3 tangent = glm::rotateY(playerDir, glm::pi<float>() * 0.5F);
	pos += ci::vec3(-tangent.x, tangent.y, -tangent.z) / 4.f;
	beginpos = pos;
	ci::vec3 normalizedSpeed = playerDir * glm::angleAxis(glm::radians(50.0F), tangent);

	float power = 10.0F;

	speed = normalizedSpeed * power;
}
void cUseWeaponCapsel::createLootPos()
{
	mLootPos.clear();
	mColorT.clear();
	auto ret = Collision::cCollisionManager::getInstance()->simulation(rigid, 30);
	for (int i = 0; i < ret.positions.size(); ++i)
	{
		mLootPos.push_back(ret.positions[i]);
		mColorT.push_back(std::fmodf((float(i)) / (float(ret.positions.size())) + deltaColor, 1.0f));
	}
}
void cUseWeaponCapsel::updateLoot(const float & delta_time)
{
}
}
}
}
