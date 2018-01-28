#pragma once
#include <Game/Weapons/MainWeapon/cBase.h>
#include <Game/Weapons/MainWeapon/cLightSaber.h>
#include <memory>
namespace Game
{
namespace Weapons
{
namespace MainWeapon
{
enum WeaponType
{
	LIGHT_SABER = 0,
};
class cFactory
{
public:
	static std::shared_ptr<cBase> create( WeaponType type, Player::cPlayer const& player )
	{
		switch ( type )
		{
		case WeaponType::LIGHT_SABER:
			return std::make_shared<cLightSaber>( player );
		default:
			return std::make_shared<cBase>( player );
		}
	}
};
}
}
}
