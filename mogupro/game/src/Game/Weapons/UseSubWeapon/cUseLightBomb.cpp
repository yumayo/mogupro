#include <Game/Weapons/UseSubWeapon/cUseLightBomb.h>
#include<Game/cSubWeaponManager.h>
#include <Utility/cInput.h>
#include<Game/cPlayerManager.h>
namespace Game
{
namespace Weapons
{
namespace UseSubWeapon
{
cUseLightBomb::cUseLightBomb( )
{

}
cUseLightBomb::~cUseLightBomb( )
{

}
void cUseLightBomb::setup(const int playerid)
{
	mPlayerId = playerid;
	Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getWhichTeam();
}

void cUseLightBomb::update(const float & delta_time)
{
	ci::app::console() << "ƒAƒvƒf" << std::endl;
	if (mIsdelete)return;

	if (!ENV->pressKey(ci::app::KeyEvent::KEY_g)) {
		createSubWeapon();
		mIsdelete = true;
		return;
	}
}
void cUseLightBomb::draw()
{

}
bool cUseLightBomb::deleteThis()
{
	return mIsdelete;
}
void cUseLightBomb::createSubWeapon()
{
	SUBWM->createLightBomb(ci::vec3(0, 0, 0), ci::vec3(0, 0, 0), ci::vec3(1, 1, 1), mPlayerId);
}
}
}
}
