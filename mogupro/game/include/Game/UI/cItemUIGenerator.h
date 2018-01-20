#pragma once
#include <Node/node.h>
#include <Game/Weapons/SubWeapon/SubWeaponType.h>
namespace Game
{
namespace UI
{
class cItemUIGenerator
{
public:
	static Utility::hardptr<Node::node> createItem( Weapons::SubWeapon::SubWeaponType type );
};
}
}
