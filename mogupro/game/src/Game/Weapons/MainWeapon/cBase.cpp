#include <Game/Weapons/MainWeapon/cBase.h>
namespace Game
{
namespace Weapons
{
namespace MainWeapon
{
cinder::mat4 cBase::getWorldMatrix( ) const
{
	return player.getWorldMatrixWeapon( );
}
}
}
}