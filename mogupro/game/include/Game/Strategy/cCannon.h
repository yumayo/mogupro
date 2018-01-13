#pragma once
#include"Game/Strategy/cStrategyObjectBase.h"
#include"Game/cGemManager.h"
#include"Node\node.h"
#include"Node\action.hpp"
#include"Game\Gem/cGemStone.h"
#include"Game/Player/cPlayer.h"
#include"Collision\cRigidBody.h"
#include"Collision\cAABBCollider.h"
#include"cinder\AxisAlignedBox.h"
#include <Game/cLightManager.h>
#include"cinder/gl/gl.h"
namespace Game
{
	namespace Strategy
	{
		class cCannon
		{
		public:
			cCannon(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 Foundationpos, const ci::vec3 Foundationscale, const Game::Player::Team team);
			~cCannon();

			static const int GEM_MAXNUM = 100;

			void draw();
			void update(const float& delta_time);
			void setup();
			Game::Player::Team getTeam();
			ci::AxisAlignedBox getAABB();
			//////////ïÛêŒÇÇ‡ÇÁÇ¢Ç‹Ç∑
			void receivePlayerGem(int getgemnum,int playerid);

			void receiveQuarryGem(std::vector<std::shared_ptr<Game::Gem::cFragmentGem>>& getgems,int playerid,bool ismyobject);
			void receiveQuarryGem(int getgemnum, int playerid, bool ismyobject);
			/////////////ïÛêŒÇíôÇﬂÇÈèÍèä
			ci::vec3 getGemStorePos();
			std::vector<std::shared_ptr<Game::Gem::cFragmentGem>>getStoregems();
			bool getIsCollectMaxGem();
			void setAddCanonPower(const int getgemnum);
		private:
			void sendCollectMaxGem();
			ci::gl::VboMeshRef mesh;
			Game::Light::PointLightHandle light;
			std::vector<std::shared_ptr<Game::Gem::cFragmentGem>>mGetgems;
			Collision::cAABBCollider mAABB;
			Collision::cAABBCollider mFoundatioAABB;
			Game::Player::Team mTeam;
			ci::AxisAlignedBox mToPlayerAABB;
			ci::vec3 mPos;
			ci::vec3 mScale;
			ci::ColorA mColor;
			ci::vec3 mFoundationPos;
			ci::vec3 mFoundationScale;
			ci::vec3 mGemStorePos;
			std::string mCannonName;
			float lightradius = 2.0f;
			float direction;
			float mLightSinAngle = 0.0f;
			bool mIsCollectMaxNum = false;
			int mGemCount = 0;
			float getSinspeed();
		};
	}
}
