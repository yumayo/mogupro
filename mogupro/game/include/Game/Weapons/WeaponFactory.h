#pragma once
#include <Game/Weapons/WeaponBase.h>
#include <Utility/cSingletonAble.h>
#include <memory>

namespace Game {
	namespace Weapon {
		enum MAIN_WEAPON {
			LIGHT_SABER = 0,
		};
		enum SUB_WEAPON {
			LIGHT_BOMB = 0,
		};

		class cWeaponFactory : public ::Utility::cSingletonAble<cWeaponFactory>
		{
		public:
			std::unique_ptr<Game::Weapon::WeaponBase> InstanceMainWeapon(MAIN_WEAPON main_weapon);
			std::unique_ptr<Game::Weapon::WeaponBase> InstanceSubWeapon(SUB_WEAPON sub_weapon);
		};
	}
}
