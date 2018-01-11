#pragma once
#include "cinder/app/App.h"
namespace Game
{
namespace Weapons
{
namespace UseSubWeapon
{
class cUseSubBase
{
public:
    cUseSubBase( );
	virtual ~cUseSubBase();
	virtual void setup(const int playerid) = 0;
	virtual void update(const float& delta_time) = 0;
	virtual void draw() = 0;
	virtual bool deleteThis();
	virtual void createSubWeapon() = 0;
	int getPlayerId();
protected:
	int mPlayerId;
private:

};
}
}
}
