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
using namespace ci;
using namespace ci::app;

using namespace Node;

using namespace Node::Action;
namespace Game
{
	namespace Strategy
	{

		cCannon::cCannon(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 Foundationpos, const ci::vec3 Foundationscale, const Game::Player::Team team)
			: mAABB(pos, scale) , mFoundatioAABB(Foundationpos, Foundationscale) {
			mPos = pos;
			mScale = scale;
			mTeam = team;
			mFoundationPos = Foundationpos;
			mFoundationScale = Foundationscale;
			mGemStorePos = mPos + ci::vec3(0,mScale.y/5.f,0);
			if (team == Game::Player::Team::Red) {
				direction = 1.f;
				mColor = ci::ColorA(1, 0, 0, 1);
				mCannonName = "cannon_red";
			}
			else {
				direction = -1.f;
				mColor = ci::ColorA(0, 0, 1, 1);
				mCannonName = "cannon_blue";
			}
		}
		cCannon::~cCannon()
		{
			mAABB.removeWorld();
			mFoundatioAABB.removeWorld();
		    //rb.removeWorld();
		}

		void cCannon::draw()
		{
			ci::gl::ScopedTextureBind tex(TEX->get("cannon"));
			gl::pushModelView();
			gl::translate(mPos);
			gl::scale(ci::vec3(mScale.x));
			gl::color(ColorA(1,1,1,1));
			Resource::cFbxManager::getInstance()->draw(mCannonName);
			gl::popModelView();


			//float rate = mScale.x;
			///////////////////土台
			//STRM->drawCube(mFoundationPos, mFoundationScale, vec3(0, 0, 0), ColorA(0.5, 0.5, 0.5, 1));
			////////////////////

			///////////////////キャノン
			//STRM->drawCube(mPos+vec3(0, rate*0.25f,direction*rate*0.4f),vec3(rate*0.3f,rate,rate*0.3f*direction), vec3(-30*direction, 0, 0), ColorA(0, 0, 0, 1));
			////////////////////

			///////////////////スフィア
			//ci::gl::pushModelView();
			//ci::gl::translate(mPos);
			//ci::gl::color(ColorA(1, 1, 1, 1));
			//ci::gl::scale(ci::vec3(mScale.x / 2.f));
			//ci::gl::draw(mesh);
			//ci::gl::popModelView();
			////STRM->drawShere(mPos, ci::vec3(mScale.x / 2.f), vec3(0, 0, 0), ColorA(1, 1, 1, 1), 30);
			///////////////////

			///////////////////本体
			//STRM->drawCube(mPos - ci::vec3(0, mScale.y / 4.f, 0), ci::vec3(mScale.x, mScale.y / 2.f, mScale.z), vec3(0, 0, 0), mColor);
			///////////////////

			////////////////AABBを描画
			//STRM->drawCube(mPos, mScale, vec3(0, 0, 0), ColorA(0, 0, 0, 1));
			//////////////////
		}

		void cCannon::update(const float & delta_time)
		{
			mLightSinAngle += delta_time*getSinspeed();
			float max = lightradius*(0.8f + 0.2f*sin(mLightSinAngle));
			light->reAttachRadius(max);
		}

		void cCannon::setup()
		{
			mAABB.addWorld();
			mFoundatioAABB.addWorld();
			light = cLightManager::getInstance()->addPointLight(mGemStorePos, ci::vec3(mColor.r, mColor.g, mColor.b) , lightradius);
			mToPlayerAABB.set(mAABB.getPosition() - mAABB.getSize()*0.55f, mAABB.getPosition() + mAABB.getSize()*0.55f);
			mesh = Resource::cObjectManager::getInstance()->findObject("sphere.obj");

			TEX->set("cannon", "Fbx/" + mCannonName + ".png");
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
				.easeTime(30.0f)
				.count(color.size()*2)
				.effectTime(0)
				.vanishTime(5.0)
				.randomEaseTypes({EaseType::BackIn,EaseType::BackOut,EaseType::CircIn,EaseType::CircOut,EaseType::Linear,EaseType::CubicIn})
			);


			Game::cClientAdapter::getInstance()->sendAddCannonPower(Game::cPlayerManager::getInstance()->getPlayers()[playerid]->getWhichTeam(), getgemnum);
			////////////
			Sound::StereophonicManager::getInstance()->add(Sound::Wav(ci::app::getAssetPath("SE/cannoncharge.wav").string()), mPos);
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
