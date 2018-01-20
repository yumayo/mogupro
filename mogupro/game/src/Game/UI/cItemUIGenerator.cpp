#include <Game/UI/cItemUIGenerator.h>
#include <Game/UI/cItemDefault.h>
#include <Game/UI/cItemQuarry.h>
#include <Game/UI/cItemLightBomb.h>
namespace Game
{
namespace UI
{
Utility::hardptr<Node::node> cItemUIGenerator::createItem( Weapons::SubWeapon::SubWeaponType type )
{
	switch ( type )
	{
	case Game::Weapons::SubWeapon::LIGHT_BOMB:
		return cItemLightBomb::create( );
	case Game::Weapons::SubWeapon::QUARRY:
		return cItemQuarry::create( );
	default:
		return cItemDefault::create( );
	}
}
}
}
