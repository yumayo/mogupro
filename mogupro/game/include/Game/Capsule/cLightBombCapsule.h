#pragma once
#include<Game/Capsule/cCapsuleBase.h>
namespace Game
{
namespace Capsule
{
class cLightBombCapsule : public cCapsuleBase
{
public:
    cLightBombCapsule( );
    ~cLightBombCapsule( );
	void setup(const ci::vec3 pos,const Game::Weapons::SubWeapon::SubWeaponType type) override;
	void update(const float& delta_time)override;
	void draw()override;
	bool deleteThis()override;
private:
	ci::vec3 rotate;
	int count = 0;
};
}
}
