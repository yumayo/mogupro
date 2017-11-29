#include<Game/Strategy/cCannon.h>
#include "cinder/gl/gl.h"
#include"Game/cStrategyManager.h"
#include"Game/cGemManager.h"
#include<Network.hpp>
#include"cinder\gl\scoped.h"
#include"Game\cClientAdapter.h"
#include"Game\cPlayerManager.h"
#include"Game\Field\FieldData.h"
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
			mGemStorePos = mPos+ci::vec3(0,mScale.y/5.f,0);
			if (team == Game::Player::Team::Red) {
				direction = 1.f;
				mColor = ci::ColorA(1, 0, 0, 1);
			}
			else {
				direction = -1.f;
				mColor = ci::ColorA(0, 0, 1, 1);
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
			float rate = mScale.x;
			/////////////////“y‘ä
			STRM->drawCube(mFoundationPos, mFoundationScale, vec3(0, 0, 0), ColorA(0.5, 0.5, 0.5, 1));
			//////////////////

			/////////////////ƒLƒƒƒmƒ“
			STRM->drawCube(mPos+vec3(0, rate*0.25f,direction*rate*0.4f),vec3(rate*0.3f,rate,rate*0.3f*direction), vec3(-30*direction, 0, 0), ColorA(0, 0, 0, 1));
			//////////////////

			/////////////////ƒXƒtƒBƒA
			STRM->drawShere(mPos, ci::vec3(mScale.x / 2.f), vec3(0, 0, 0), ColorA(1, 1, 1, 1), 30);
			/////////////////

			/////////////////–{‘Ì
			STRM->drawCube(mPos - ci::vec3(0, mScale.y / 4.f, 0), ci::vec3(mScale.x, mScale.y / 2.f, mScale.z), vec3(0, 0, 0), mColor);
			/////////////////

			////////////////AABB‚ð•`‰æ
			//STRM->drawCube(mPos, mScale, vec3(0, 0, 0), ColorA(0, 0, 0, 1));
			//////////////////
		}

		void cCannon::update(const float & delta_time)
		{

		}

		void cCannon::setup()
		{
			mAABB.addWorld();
			mFoundatioAABB.addWorld();
		}

		Game::Player::Team cCannon::getTeam()
		{
			return mTeam;
		}

		ci::AxisAlignedBox cCannon::getAABB()
		{
			return mToPlayerAABB;
		}

		void cCannon::receivePlayerGem( const std::vector<std::shared_ptr<Game::Gem::cGemStone>> getgems)
		{
			ci::app::console() << "‚¢‚¦‚é‚à‚Ì‚Í‚¶‚ß" << std::endl;
			for (int i = 0; i < getgems.size(); i++) {
				ci::app::console() << getgems[i]->getType() << std::endl;
				mGetgems.push_back(getgems[i]);
			}
			ci::app::console() << "‚¢‚¦‚é‚à‚Ì‚¨‚í‚è" << std::endl;


			ci::app::console() << "‚º‚ñ‚Ô‚Í‚¶‚ß" << std::endl;
			for (int i = 0; i < mGetgems.size(); i++) {
				ci::app::console() << mGetgems[i]->getType() << std::endl;
			}
			ci::app::console() << "‚º‚ñ‚Ô‚¨‚í‚è" << std::endl;
		}
		ci::vec3 cCannon::getGemStorePos()
		{
			return mGemStorePos;
		}

		std::vector<std::shared_ptr<Game::Gem::cGemStone>> cCannon::getStoregems()
		{
			return mGetgems;
		}

	}
}
