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
public:
	// TIPS—p
	bool isHitGem( );
private:
	#define CHAIN(type, var, def) type m_ ## var = def; float m_ ## var ## Time = -1.0F; TumeFormat& var(float time, type var){ m_ ## var = var; m_ ## var ## Time = time; return *this; }
	struct TumeFormat
	{
		CHAIN( float, tumeRootRotation, 0.0F );
		CHAIN( ci::vec3, tumeRootAxis, ci::vec3( 0, 1, 0 ) );
		CHAIN( ci::vec3, tumeRootPos, ci::vec3( 0, 0, 0 ) );
		CHAIN( float, tumeRotation, 0.0F );
		CHAIN( ci::vec3, tumeAxis, ci::vec3( 0, 0, 1 ) );
		CHAIN( ci::vec3, tumePos, ci::vec3( 0, 0, 0.5F ) );
	};
	#undef CHAIN
	void animation( float t, TumeFormat const& tumeFormat );
	void addBullet( float lightRadius, float damage );
private:
	Utility::cStateMachineBehaviour stateMachine;
	std::function<void( )> debugDrawFunc;
	hardptr<Node::node> tumeRoot;
	softptr<Node::node> tumeHolder;
	softptr<Node::node> tume;
	hardptr<Node::node> tumeBulletRoot;
	ci::vec3 pos;
	float delta = 0.0F;
};
}
}
}