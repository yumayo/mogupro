#include <Game/Weapons/MainWeapon/cBase.h>
namespace Game
{
namespace Weapons
{
namespace MainWeapon
{
void cBase::pushCall( )
{
	push = true;
	press = true;
}
void cBase::pullCall( )
{
	ci::app::console( ) << "pullCall" << std::endl;
	pull = true;
	press = false;
}
void cBase::flashInput( )
{
	push = false;
	pull = false;
}
cinder::mat4 cBase::getWorldMatrix( ) const
{
	return player.getWorldMatrixWeapon( );
}
}
}
}