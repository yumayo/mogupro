#include <Game/Weapons/WeaponFactory.h>
#include <Game/Weapons/MainWeapon/LightSaber.h>

std::unique_ptr<Game::Weapon::WeaponBase> Game::Weapon::cWeaponFactory::InstanceMainWeapon(MAIN_WEAPON main_weapon)
{
	switch (main_weapon)
	{
	case LIGHT_SABER:
		return std::make_unique<Game::Weapon::LightSaber>();
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
	case BOMB:
		return NULL;
		break;
	default:
		break;
	}
	return NULL;
}
