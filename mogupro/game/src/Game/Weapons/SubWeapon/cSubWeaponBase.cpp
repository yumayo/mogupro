#include <Game/Weapons/SubWeapon/cSubWeaponBase.h>
namespace Game
{
namespace Weapons
{
namespace SubWeapon
{
cSubWeaponBase::cSubWeaponBase( )
{

}
cSubWeaponBase::~cSubWeaponBase( )
{

}
bool cSubWeaponBase::deleteThis()
{
	return false;
}
int cSubWeaponBase::getId()
{
	return mPlayerId;
}
}
}
}
