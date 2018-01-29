#pragma once
#include <Game/Weapons/MainWeapon/cBase.h>
#include <Utility/cStateMachineBehaviour.h>
namespace Game
{
namespace Weapons
{
namespace MainWeapon
{
class cLightSaber : public cBase
{
public:
	cLightSaber( Player::cPlayer& player );
public: //! override functions
	void setup( ) override;
	void update( const float& delta_time ) override;
	void draw( ) override;
	void reset( ) override;
private:
	Utility::cStateMachineBehaviour stateMachine;
	std::function<void( )> drawFunc;
	cinder::vec3 pos;
};
}
}
}