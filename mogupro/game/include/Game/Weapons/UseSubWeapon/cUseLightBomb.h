#pragma once
#include<Game/Weapons/UseSubWeapon/cUseSubBase.h>
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
	bool mIsdelete = false;
};
}
}
}
