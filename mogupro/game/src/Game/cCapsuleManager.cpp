#include <Game/cCapsuleManager.h>
#include<Game/Capsule/cLightBombCapsule.h>
#include<Game/Capsule/cQuarryCapsule.h>
#include<Game/Field/FieldData.h>
#include<Game/cPlayerManager.h>
//#include<Game/Weapons/SubWeapon/SubWeaponType.h>
namespace Game
{
cCapsuleManager::cCapsuleManager( )
{

}
cCapsuleManager::~cCapsuleManager( )
{

}
void cCapsuleManager::setup()
{
	ci::vec3 pos = ci::vec3(1, Game::Field::CHUNK_RANGE_Y*Game::Field::CHUNK_SIZE*Game::Field::BLOCK_SIZE,1);
	createCapsule(pos, Game::Weapons::SubWeapon::LIGHT_BOMB,debugcount);
}
void cCapsuleManager::draw()
{
	for (auto& itr : mCapsules) {
		itr.second->draw();
	}
}
void cCapsuleManager::update(const float & deltatime)
{
	for (auto& itr : mCapsules) {
		itr.second->update(deltatime);
	}
	deleteCapsule();
	CollisionToPlayer();
}
void cCapsuleManager::deleteCapsule()
{
	for (auto itr = mCapsules.begin();
		itr != mCapsules.end();) {
		if (itr->second->deleteThis()) {
			itr = mCapsules.erase(itr);
		}
		else {
			itr++;
		}
	}
}
void cCapsuleManager::createCapsule(const ci::vec3 pos, const Game::Weapons::SubWeapon::SubWeaponType type, const int capsuleid)
{
	switch (type)
	{
	case Game::Weapons::SubWeapon::LIGHT_BOMB:
		mCapsules.insert(std::make_pair(capsuleid, std::make_shared<Game::Capsule::cLightBombCapsule>()));
		break;
	default:
		ci::app::console() << "カプセルが予想外です" << std::endl;
		return;
	}
	mCapsules[capsuleid]->setup(pos, type);
	debugcount++;
}

void cCapsuleManager::CollisionToPlayer()
{
	for (auto& itr : mCapsules) {
		if (itr.second->getIsget())continue;
		if (Game::cPlayerManager::getInstance()->getActivePlayer()->getAABB().intersects(itr.second->getAABB())) {
			////////////////////////通信
			HitPlayer(Game::cPlayerManager::getInstance()->getActivePlayer()->getPlayerId(), itr.first);
			////////////////////////通信
		}
	}
}
void cCapsuleManager::HitPlayer(const int playerid, const int capsuleid)
{
	mCapsules[capsuleid]->setIsget(true);
	Game::cPlayerManager::getInstance()->getPlayers()[playerid]->useSubWeapon.addSubWeapon(mCapsules[capsuleid]->getType());
}
}
