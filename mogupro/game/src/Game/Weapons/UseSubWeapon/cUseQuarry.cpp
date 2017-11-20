#include <Game/Weapons/UseSubWeapon/cUseQuarry.h>
#include<Game/cPlayerManager.h>
#include"Resource\TextureManager.h"
#include"cinder\gl\scoped.h"
#include "cinder/gl/gl.h"
#include<Game/cStrategyManager.h>
#include<Game/cFieldManager.h>
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
	mScale = ci::vec3(1, 1, 1);
}
void cUseQuarry::calcPos(const float length, const int playerid) {

	mSetPos = Game::cFieldManager::getInstance()->getBlockTopPosition(Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPos()
		+ Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPlayerVec());

	/*mSetPos = Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPos()
		+ Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPlayerVec();*/
	/*ci::vec3 direction = glm::normalize(cPlayerManager::getInstance()->getPlayers()[playerid]->getInstallationPosition());

	mSetPos = cPlayerManager::getInstance()->getPlayers()[playerid]->getPos() + length*direction;*/
}
void cUseQuarry::draw() {
	gl::enableAlphaBlending();
	cinder::gl::ScopedTextureBind a(TEX->get("drill.png"));
	gl::ScopedGlslProg shader(gl::getStockShader(gl::ShaderDef().texture()));
	STRM->drawCube(mSetPos, ci::vec3(mScale.x*1.1f, mScale.y*1.1f, mScale.z*1.1f), ci::vec3(0, 0, 0), ci::ColorA(1, 1, 1, 0.5f));
	//gl::disableAlphaBlending();
}
bool cUseQuarry::deleteThis() {

	return false;
}
void cUseQuarry::createSubWeapon() {


}
}
}
}
