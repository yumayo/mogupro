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
	createCapsule(pos, Game::Weapons::SubWeapon::LIGHT_BOMB);
}
void cCapsuleManager::draw()
{
	for (auto& itr : mCapsules) {
		itr->draw();
	}
}
void cCapsuleManager::update(const float & deltatime)
{
	for (auto& itr : mCapsules) {
		itr->update(deltatime);
	}
	deleteCapsule();
	CollisionToPlayer();
}
void cCapsuleManager::deleteCapsule()
{
	for (auto itr = mCapsules.begin();
		itr != mCapsules.end();) {
		if ((*itr)->deleteThis()) {
			itr = mCapsules.erase(itr);
		}
		else {
			itr++;
		}
	}
}
void cCapsuleManager::createCapsule(const ci::vec3 pos, const Game::Weapons::SubWeapon::SubWeaponType type)
{
	switch (type)
	{
	case Game::Weapons::SubWeapon::LIGHT_BOMB:
		mCapsules.push_back(std::make_shared<Game::Capsule::cLightBombCapsule>());
		break;
	default:
		ci::app::console() << "カプセルが予想外です" << std::endl;
		return;
	}
	mCapsules.back()->setup(pos, type);
}
void cCapsuleManager::CollisionToPlayer()
{
	for (auto itr = mCapsules.begin();
		itr != mCapsules.end(); itr++) {
		if ((*itr)->getIsget())continue;

		if (Game::cPlayerManager::getInstance()->getActivePlayer()->getAABB().intersects((*itr)->getAABB())) {
			(*itr)->setIsget(true);
			Game::cPlayerManager::getInstance()->getActivePlayer()->useSubWeapon.addSubWeapon((*itr)->getType());
		}
	}
}
}
