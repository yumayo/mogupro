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
	cBase( Player::cPlayer& player ) : player( player ) { }
public:
	virtual ~cBase( ) { }
	virtual void setup( ) { };
	virtual void update( const float& delta_time ) { };
	virtual void draw(cinder::mat4 const& m) { };
	virtual void reset( ) { };
public:
	void pushCall( );
	void pullCall( );
	void flashInput( );
	cinder::mat4 getWorldMatrix( ) const;
	bool getPress( ) const { return press; }
protected:
	Player::cPlayer& player;
	bool push = false;
	bool press = false;
	bool pull = false;
};
}
}
}