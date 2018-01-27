#include <Game/Weapons/cWeaponFactory.h>
#include <Game/Weapons/MainWeapon/cLightSaber.h>
namespace Game
{
namespace Weapons
{
std::unique_ptr<Game::Weapons::cWeaponBase> cWeaponFactory::InstanceMainWeapon( MAIN_WEAPON main_weapon, int player_id )
{
	switch ( main_weapon )
	{
	case LIGHT_SABER:
		return std::make_unique<MainWeapon::cLightSaber>( player_id );
		break;
	default:
		break;
	}
	return NULL;
}
std::unique_ptr<Game::Weapons::cWeaponBase> cWeaponFactory::InstanceSubWeapon( SUB_WEAPON sub_weapon )
{
	switch ( sub_weapon )
	{
	case LIGHT_BOMB:
		return NULL;
		break;
	default:
		break;
	}
	return NULL;
}
}
}

