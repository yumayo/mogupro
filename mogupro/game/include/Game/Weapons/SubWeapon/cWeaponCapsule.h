#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
#include"Collision\cRigidBody.h"
#include"Collision\cAABBCollider.h"
#include"Game/Weapons/SubWeapon/SubWeaponType.h"
namespace Game
{
namespace Weapons
{
namespace SubWeapon
{
class cWeaponCapsule
{
public:
    cWeaponCapsule(const ci::vec3 pos, const ci::vec3 speed, const int playerid, const int objectid, const Game::Weapons::SubWeapon::SubWeaponType type);
	~cWeaponCapsule();
private:
	Collision::cRigidBody rb;
	Collision::cAABBCollider aabb;
public:
	void setup();
	void update(const float& delta_time);
	void updateCollisionAfterUpdate(const float& delta_time);
	void draw();
	bool deleteThis();
	int getPlayerId();
	int getObjectId();
	bool ismyobject;
	ci::vec3 getPos();
	ci::vec3 getScale();
private:
	void HitLand();
private:
	ci::vec3 mPos;
	ci::vec3 mScale = ci::vec3(0.5);
	ci::vec3 mSpeed;
	ci::vec3 mRotate = ci::vec3(0);
	float rotatez = 0.0f;
	int mTeamNum;
	ci::gl::VboMeshRef mesh;
	ci::vec3 mDrawScale;
	Game::Weapons::SubWeapon::SubWeaponType mType;
	bool isDelete = false;
	bool mIsHitLand = false;
	bool mIsMyObject = false;
	int mPlayerId;
	int mObjectId;
	void createSubWeapon();
};
}
}
}
