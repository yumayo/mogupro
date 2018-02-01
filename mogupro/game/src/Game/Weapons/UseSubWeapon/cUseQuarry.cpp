#include <Game/Weapons/UseSubWeapon/cUseQuarry.h>
#include<Game/cPlayerManager.h>
#include"Resource\TextureManager.h"
#include"cinder\gl\scoped.h"
#include "cinder/gl/gl.h"
#include<Game/cStrategyManager.h>
#include<Game/cFieldManager.h>
#include <Utility/cInput.h>
#include<Game/Weapons/SubWeapon/cQuarry.h>
#include<Game/cSubWeaponManager.h>
#include<Game/cClientAdapter.h>
using namespace ci;
using namespace ci::app;
namespace Game
{
namespace Weapons
{
namespace UseSubWeapon
{
cUseQuarry::cUseQuarry( )
{
	TEX->set("drill.png", "drill.png");
}
cUseQuarry::~cUseQuarry( )
{

}
void cUseQuarry::setup(const int playerid) {
	mPlayerId = playerid;
	mLength = 2.0f;

	calcPos(mLength,playerid);
	
	Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getWhichTeam();
}
void cUseQuarry::update(const float& delta_time) {
	calcPos(mLength, mPlayerId);
	mScale = ci::vec3(3, 3, 3);
	if (!ENV->pressKey(ci::app::KeyEvent::KEY_g)) {
		createSubWeapon();
		mIsdelete = true;
	}
}
void cUseQuarry::calcPos(const float length, const int playerid) {

	mSetPos = Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPos()
		+ glm::normalize(Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPlayerVec())*mScale.x;

	/*mSetPos = Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPos()
		+ Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPlayerVec();*/
	/*ci::vec3 direction = glm::normalize(cPlayerManager::getInstance()->getPlayers()[playerid]->getInstallationPosition());

	mSetPos = cPlayerManager::getInstance()->getPlayers()[playerid]->getPos() + length*direction;*/
}
void cUseQuarry::draw() {
	gl::enableAlphaBlending();
	cinder::gl::ScopedTextureBind a(TEX->get("drill.png"));
	gl::ScopedGlslProg shader(gl::getStockShader(gl::ShaderDef().texture()));
	STRM->drawCube(mSetPos + ci::vec3(0,mScale.y/2.f,0), ci::vec3(mScale.x, mScale.y, mScale.z), ci::vec3(0, 0, 0), ci::ColorA(1, 1, 1, 0.5f));
	//gl::disableAlphaBlending();
}
bool cUseQuarry::deleteThis() {

	return mIsdelete;
}
void cUseQuarry::updateCollisionAfterUpdate(const float& delta_time) {

}
void cUseQuarry::createSubWeapon() {

	Game::cClientAdapter::getInstance( )->sendSetQuarry( mSetPos + ci::vec3( 0, mScale.y / 2.f, 0 ) );

	//Game::cSubWeaponManager::getInstance()->createQuarry(mSetPos + ci::vec3(0, mScale.y / 2.f, 0), cSubWeaponManager::getInstance()->debugidcount,
	//	cPlayerManager::getInstance()->getActivePlayerId());
}
}
}
}
