#pragma once
#include"Utility\cSingletonAble.h"
#include<Game/Capsule/cCapsuleBase.h>
#include<Game/Weapons/SubWeapon/SubWeaponType.h>
namespace Game
{
class cCapsuleManager : public Utility::cSingletonAble<cCapsuleManager>
{
public:
    cCapsuleManager( );
    ~cCapsuleManager( );
	void setup();
	void draw();
	void update(const float& deltatime);
	void deleteCapsule();
	void createCapsule(const ci::vec3 pos,const Game::Weapons::SubWeapon::SubWeaponType type);
	void CollisionToPlayer();
private:
	std::list<std::shared_ptr<Game::Capsule::cCapsuleBase>>mCapsules;
};
}
#define CAPSULEM Game::cCapsuleManager::getInstance()