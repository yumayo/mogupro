#pragma once
#include "cinder/app/App.h"
#include "cinder/gl/gl.h"
namespace Game
{
namespace Weapons
{
namespace SubWeapon
{
class cSubWeaponBase
{
public:
    cSubWeaponBase( );
    virtual ~cSubWeaponBase( );
	virtual void setup() = 0;
	virtual void update(const float& delta_time) = 0;
	virtual void updateCollisionAfterUpdate(const float& delta_time) = 0;
	virtual void draw() = 0;
	virtual bool deleteThis();
	int getPlayerId();
	int getObjectId();
	bool ismyobject;
protected:
	ci::vec3 mPos = ci::vec3(0, 0, 0);
	ci::vec3 mScale = ci::vec3(1, 1, 1);
	int mPlayerId;
	int mObjectId;
private:

};
}
}
}
