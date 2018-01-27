#pragma once
#include <Game/Weapons/cWeaponBase.h>
#include <Utility/cSingletonAble.h>
#include <memory>
namespace Game
{
namespace Weapons
{
enum MAIN_WEAPON
{
	LIGHT_SABER = 0,
};
enum SUB_WEAPON
{
	LIGHT_BOMB = 0,
};
class cWeaponFactory : public Utility::cSingletonAble<cWeaponFactory>
{
public:
	std::unique_ptr<Game::Weapons::cWeaponBase> InstanceMainWeapon( MAIN_WEAPON main_weapon, int player_id );
	std::unique_ptr<Game::Weapons::cWeaponBase> InstanceSubWeapon( SUB_WEAPON sub_weapon );
};
}
}
