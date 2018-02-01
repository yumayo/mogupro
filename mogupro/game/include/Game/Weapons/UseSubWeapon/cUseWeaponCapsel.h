#pragma once
#include<Game/Weapons/UseSubWeapon/cUseSubBase.h>
#include"cinder\gl\gl.h"
#include"Collision\cRigidBody.h"
#include"Collision\cAABBCollider.h"
#include <Game/cLightManager.h>
#include"Game/Weapons/SubWeapon/SubWeaponType.h"
namespace Game
{
namespace Weapons
{
namespace UseSubWeapon
{
class cUseWeaponCapsel
{
public:
    cUseWeaponCapsel( );
    ~cUseWeaponCapsel( );
	void setup(const int playerid,const Game::Weapons::SubWeapon::SubWeaponType type);
	void update(const float& delta_time);
	void updateCollisionAfterUpdate(const float& delta_time);
	void draw();
	bool deleteThis();
	void createSubWeapon();
	int getPlayerId();
private:
	void setSpeed();
	void createLootPos();
	void updateLoot(const float& delta_time);
	bool mIsdelete = false;
	int mPlayerId;
	Game::Weapons::SubWeapon::SubWeaponType mType;
	Game::Light::PointLightHandle light;
	Collision::cAABBCollider collider;
	Collision::cRigidBody rigid;
	std::vector<ci::vec3>mLootPos;
	std::vector<float>mColorT;
	ci::vec3 speed;
	ci::vec3 beginpos;
	float deltaColor = 0.0f;
	float sincolor = 0.0f;
};
}
}
}
