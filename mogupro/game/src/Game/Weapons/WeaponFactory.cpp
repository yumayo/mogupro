#include <Game/Weapons/WeaponFactory.h>
#include <Game/Weapons/MainWeapon/LightSaber.h>

std::unique_ptr<Game::Weapon::WeaponBase> Game::Weapon::cWeaponFactory::InstanceMainWeapon(MAIN_WEAPON main_weapon,int player_id)
{
	switch (main_weapon)
	{
	case LIGHT_SABER:
		return std::make_unique<Game::Weapon::LightSaber>(player_id);
		break;
	default:
		break;
	}
	return NULL;
	
}

std::unique_ptr<Game::Weapon::WeaponBase> Game::Weapon::cWeaponFactory::InstanceSubWeapon(SUB_WEAPON sub_weapon)
{
	switch (sub_weapon)
	{
	case LIGHT_BOMB:
		return NULL;
		break;
	default:
		break;
	}
	return NULL;
}
