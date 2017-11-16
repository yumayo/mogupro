#include <Game/Weapons/UseSubWeapon/cUseSubBase.h>
namespace Game
{
namespace Weapons
{
namespace UseSubWeapon
{
cUseSubBase::cUseSubBase( )
{

}
cUseSubBase::~cUseSubBase( )
{

}
bool cUseSubBase::deleteThis()
{
	return false;
}
int cUseSubBase::getPlayerId()
{
	return mPlayerId;
}
}
}
}
