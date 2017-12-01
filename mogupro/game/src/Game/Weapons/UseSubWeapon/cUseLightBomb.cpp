#include <Game/Weapons/UseSubWeapon/cUseLightBomb.h>
#include<Game/cSubWeaponManager.h>
#include <Utility/cInput.h>
#include<Game/cPlayerManager.h>
#include <Game/cClientAdapter.h>

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
	//ci::app::console() << "ƒAƒvƒf" << std::endl;
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
	ci::vec3 playerDir = cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getInstallationPosition();
	playerDir = glm::normalize(playerDir);
	ci::vec3 pos = cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getPos();
	ci::vec3 tangent = glm::rotateY(playerDir, glm::pi<float>() * 0.5F);

	ci::vec3 normalizedSpeed = playerDir * glm::angleAxis(glm::radians(50.0F), tangent);

	float power = 10.0F;

	cClientAdapter::getInstance( )->sendLightBomb( pos, normalizedSpeed * power );
}
}
}
}
