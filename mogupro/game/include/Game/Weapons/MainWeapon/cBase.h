#pragma once
#include <Game/Player/cPlayer.h>
namespace Game
{
namespace Weapons
{
namespace MainWeapon
{
class cBase
{
public:
	cBase( Player::cPlayer const& player ) : player( player ) { }
public:
	virtual ~cBase( ) { }
	virtual void pushCall( ) { push = true; press = true; };
	virtual void pullCall( ) { pull = true; press = false; };
	virtual void setup( ) { };
	virtual void update( const float& delta_time ) { };
	virtual void draw( ) { };
	virtual void reset( ) { };
public:
	void flashInput( ) { push = false; pull = false; }
	cinder::mat4 getWorldMatrix( ) const;
protected:
	Player::cPlayer const& player;
	bool push = false;
	bool press = false;
	bool pull = false;
};
}
}
}