#include<Game/Strategy/cCannon.h>
#include "cinder/gl/gl.h"
#include"Game/cStrategyManager.h"
#include"Game/cGemManager.h"
#include<Network.hpp>
#include"Game/cClientAdapter.h"
#include"cinder\gl\scoped.h"
#include"Game\cClientAdapter.h"
#include"Game\cPlayerManager.h"
#include"Resource\cObjectManager.h"
#include"Resource\cSoundManager.h"
#include"Particle\cParticleManager.h"
#include"cinder\Rand.h"
#include"Sound/Wav.h"
#include"Sound\Stereophonic.h"
#include"Resource\cFbxManager.h"
#include"Resource\TextureManager.h"
#include"Resource\cJsonManager.h"
#include"Utility\cInput.h"
using namespace ci;
using namespace ci::app;

using namespace Node;

using namespace Node::Action;
namespace Game
{
	namespace Strategy
	{

		cCannon::cCannon(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 Foundationpos, const ci::vec3 Foundationscale, const ci::vec3 Foundationpos2, const ci::vec3 Foundationscale2, const ci::vec3 storepos, ci::vec3 hitpos, ci::vec3 hitscale, const Game::Player::Team team)
			: mAABB(hitpos, hitscale), mFoundatioAABB(Foundationpos, Foundationscale), mFoundatioAABB2(Foundationpos2, Foundationscale2) {
			mPos = pos;
			mScale = scale;
			mTeam = team;
			mFoundationPos = Foundationpos;
			mFoundationScale = Foundationscale;
			mGemStorePos = storepos;
			if (team == Game::Player::Team::Red) {
				mColor = ci::ColorA(1, 0, 0, 1);
				mCannonName = "cannon_red";
			}
			else {
				mColor = ci::ColorA(0, 0, 1, 1);
				mCannonName = "cannon_blue";
			}
		}
		cCannon::~cCannon()
		{
			mAABB.removeWorld();
			mFoundatioAABB.removeWorld();
			mFoundatioAABB2.removeWorld();
		    //rb.removeWorld();
		}

		void cCannon::draw()
		{
			
			if (debugaabb) {
				//STRM->drawCube(mGemStorePos, ci::vec3(1.5, 1.5, 1.5), vec3(0, 0, 0), ColorA(0, 0, 1, 1));
			/*	STRM->drawCube(mAABB.getPosition(), mAABB.getSize(), vec3(0, 0, 0), ColorA(1, 1, 1, 1));
				
				STRM->drawCube(mFoundatioAABB.getPosition(), mFoundatioAABB.getSize(), vec3(0, 0, 0), ColorA(0, 1, 0, 1));
				STRM->drawCube(mFoundatioAABB2.getPosition(), mFoundatioAABB2.getSize(), vec3(0, 0, 0), ColorA(0, 1, 1, 1));*/
			}
		

			ci::gl::ScopedTextureBind tex(TEX->get(mCannonName));
			gl::pushModelView();
			gl::translate(mPos);
			gl::scale(ci::vec3(mScale.x));
			gl::color(ColorA(1,1,1,1));
			Resource::cFbxManager::getInstance()->draw(mCannonName);
			gl::popModelView();


		}

		void cCannon::update(const float & delta_time)
		{
			/*if (ENV->pushKey(ci::app::KeyEvent::KEY_n)) {
				debugaabb = (!debugaabb);
			}*/
		}

		void cCannon::setup()
		{
			mAABB.addWorld();
			mFoundatioAABB.addWorld();
			mFoundatioAABB2.addWorld();
			light = cLightManager::getInstance()->addPointLight(mGemStorePos, ci::vec3(mColor.r, mColor.g, mColor.b) , lightradius);
			mToPlayerAABB.set(mAABB.getPosition() - mAABB.getSize()*0.55f, mAABB.getPosition() + mAABB.getSize()*0.55f);
			mesh = Resource::cObjectManager::getInstance()->findObject("sphere.obj");

			TEX->set(mCannonName, "Fbx/" + mCannonName + ".png");
		}

		Game::Player::Team cCannon::getTeam()
		{
			return mTeam;
		}

		ci::AxisAlignedBox cCannon::getAABB()
		{
			return mToPlayerAABB;
		}

		void cCannon::receivePlayerGem(int getgemnum, int playerid)
		{
			if (getgemnum == 0) return;
			if (mGemCount >= GEM_MAXNUM)return;
		
			ci::vec3 playerpos = Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getPos();

			std::vector<ci::ColorA> color;
			
			for (int i = 0; i < getgemnum; i++) {
				color.push_back(ci::ColorA(ci::randFloat(1.0f), ci::randFloat(1.0f), ci::randFloat(1.0f), 1));
			}

			Particle::cParticleManager::getInstance()->create(Particle::ParticleParam()
				.position(playerpos)
				.moveType(Particle::ParticleType::ABSORB)
				.textureType(Particle::ParticleTextureType::SPARK)
				.colors(color)
				.convergePoint(mGemStorePos)
				.speed(0.5f)
				.swellEndTime(0.1f)
				.swellWaitTime(30.0f)
				.easeTime(60.0f)
				.count(color.size()*2)
				.effectTime(0)
				.vanishTime(5.0)
				.randomEaseTypes({EaseType::BackIn,EaseType::BackOut,EaseType::CircIn,EaseType::CircOut,EaseType::Linear,EaseType::CubicIn})
			);


			Game::cClientAdapter::getInstance()->sendAddCannonPower(Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getWhichTeam(), getgemnum);
			////////////
			Sound::StereophonicManager::getInstance()->add(mCannonName,ci::app::getAssetPath("SE/cannoncharge.wav").string(), mPos);
			////////////
			//sendCollectMaxGem();
		}

		void cCannon::receiveQuarryGem(std::vector<std::shared_ptr<Game::Gem::cFragmentGem>>& getgems, int playerid, bool ismyobject)
		{
			if (getgems.size() == 0) return;
			if (mGemCount >= GEM_MAXNUM)return;

			if (ismyobject) {
			
				Game::cClientAdapter::getInstance()->sendAddCannonPower(Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getWhichTeam(), getgems.size());
				Resource::cSoundManager::getInstance()->findSe("cannoncharge.wav").setGain(0.4f);
				Resource::cSoundManager::getInstance()->findSe("cannoncharge.wav").play();
			}
			else {
				setAddCanonPower(mGetgems.size());
			}

			ci::app::console() << "いれるものはじめ" << std::endl;
			for (int i = 0; i < getgems.size(); i++) {
				ci::app::console() << getgems[i]->getType() << std::endl;
				mGetgems.push_back(getgems[i]);
			}
			ci::app::console() << "いれるものおわり" << std::endl;

			getgems.clear();

			//////////////////////デバック
			ci::app::console() << "ぜんぶはじめ" << std::endl;
			for (int i = 0; i < mGetgems.size(); i++) {
				ci::app::console() << mGetgems[i]->getType() << std::endl;
			}
			ci::app::console() << "ぜんぶおわり" << std::endl;
			//////////////////////デバック

			sendCollectMaxGem();
		}

		void cCannon::receiveQuarryGem(int getgemnum, int playerid, bool ismyobject)
		{
			if (getgemnum == 0) return;
			if (mGemCount >= GEM_MAXNUM)return;

			if (ismyobject) {
			
				Game::cClientAdapter::getInstance()->sendAddCannonPower(Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getWhichTeam(),getgemnum);
				Resource::cSoundManager::getInstance()->findSe("cannoncharge.wav").setGain(0.4f);
				Resource::cSoundManager::getInstance()->findSe("cannoncharge.wav").play();
			}
			else {
				setAddCanonPower(getgemnum);
			}
		}
	
		ci::vec3 cCannon::getGemStorePos()
		{
			return mGemStorePos;
		}

		std::vector<std::shared_ptr<Game::Gem::cFragmentGem>> cCannon::getStoregems()
		{
			return mGetgems;
		}

		bool cCannon::getIsCollectMaxGem()
		{
			return mGetgems.size() >= GEM_MAXNUM;
		}

		void cCannon::setAddCanonPower(const int getgemnum)
		{
			lightradius += (float(getgemnum) / GEM_MAXNUM)*6.f;
			light->reAttachRadius(lightradius);
			mGemCount += getgemnum;
			if (mGemCount >= GEM_MAXNUM)mGemCount = GEM_MAXNUM;
		}

		int Game::Strategy::cCannon::getGEmNum()
		{
			return mGemCount;
		}

		void Game::Strategy::cCannon::setGemNum(const int gemnum)
		{
			mGemCount = gemnum;
		}

		ci::vec3 & Game::Strategy::cCannon::getReStorePos()
		{
			return mGemStorePos;
		}

		void cCannon::sendCollectMaxGem()
		{
			if (mGetgems.size() >= GEM_MAXNUM) {

			}
		}

		float cCannon::getSinspeed()
		{
			float speed = 2.5f;
			if (mGemCount > GEM_MAXNUM*0.8f) {
				speed = 8.f;
			}
			else if(mGemCount > GEM_MAXNUM*0.5f){
				speed = 4.f;
			}
			else {
				speed = 2.5f;
			}
			return speed;
		}

	}
}
