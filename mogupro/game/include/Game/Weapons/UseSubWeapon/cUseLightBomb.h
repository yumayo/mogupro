#pragma once
#include<Game/Weapons/UseSubWeapon/cUseSubBase.h>
#include"cinder\gl\gl.h"
#include"Collision\cRigidBody.h"
#include"Collision\cAABBCollider.h"
#include <Game/cLightManager.h>
namespace Game
{
namespace Weapons
{
namespace UseSubWeapon
{
class cUseLightBomb : public Game::Weapons::UseSubWeapon::cUseSubBase
{
public:
    cUseLightBomb( );
    ~cUseLightBomb( );
	void setup(const int playerid) override;
	void update(const float& delta_time) override;
	void draw() override;
	bool deleteThis() override;
	void createSubWeapon() override;
private:
	void setSpeed();
	void createLootPos();
	void updateLoot(const float& delta_time);
	bool mIsdelete = false;
	Game::Light::PointLightHandle light;
	Collision::cAABBCollider collider;
	Collision::cRigidBody rigid;
	std::vector<ci::vec3>mLootPos;
	std::vector<float>mColorT;
	ci::vec3 speed;
	float deltaColor = 0.0f;
	float sincolor = 0.0f;
};
}
}
}
