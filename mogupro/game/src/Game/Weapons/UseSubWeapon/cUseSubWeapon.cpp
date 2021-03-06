#include <Game/Weapons/UseSubWeapon/cUseSubWeapon.h>
#include<Game/cSubWeaponManager.h>
#include <Game/cGameManager.h>
namespace Game
{
namespace Weapons
{
namespace UseSubWeapon
{
cUseSubWeapon::cUseSubWeapon( )
{

}
cUseSubWeapon::~cUseSubWeapon( )
{

}
void cUseSubWeapon::setup()
{
	addSubWeapon(Game::Weapons::SubWeapon::SubWeaponType::LIGHT_BOMB);
}
void cUseSubWeapon::useWeapon(const int playerid)
{
	if (!mSubWeaponTypes.empty()) {
		SUBWM->createUseSubWeapon(mSubWeaponTypes[0],playerid);
		cGameManager::getInstance( )->useItem( );
		mSubWeaponTypes.erase(mSubWeaponTypes.begin());
	}
}
void cUseSubWeapon::addSubWeapon(const Game::Weapons::SubWeapon::SubWeaponType type)
{
	if (mSubWeaponTypes.size() >= 2)return;
	cGameManager::getInstance( )->appendItem( type );
	mSubWeaponTypes.push_back(type);
}
void cUseSubWeapon::clearSubWeapon()
{
	mSubWeaponTypes.clear();
	cGameManager::getInstance()->useItem();
	cGameManager::getInstance()->useItem();
	cGameManager::getInstance()->useItem();
}
void cUseSubWeapon::setCanUseWeapon(const bool iscanuse)
{
	mIsCanUse = iscanuse;
}
int cUseSubWeapon::getItemNum()
{
	return mSubWeaponTypes.size();
}
bool cUseSubWeapon::canGetItem()
{
	return mSubWeaponTypes.size() < 2;
}
}
}
}
