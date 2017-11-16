#pragma once
#include <Game/Weapons/SubWeapon/cSubWeaponBase.h>
#include <Game/cLightManager.h>
#include"Collision\cRigidBody.h"
#include"Collision\cAABBCollider.h"
namespace Game
{
	namespace Weapons
	{
		namespace SubWeapon {

			class cLightBomb : public Game::Weapons::SubWeapon::cSubWeaponBase
			{
			public:
				cLightBomb(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 speed, const int playerid);
				~cLightBomb();
			private:
				ci::vec3 mSpeed;
				Collision::cRigidBody rb;
				Collision::cAABBCollider aabb;
				int mTeamNum;

			public://通信関係
				void dmageToPlayer(const int playerid);
			private:////着地関連
				bool mIsHitObject = false;
				void hitObject();
				float mLandcount = 0.0f;

			private:////光関連
				void updateLight(const float& delta_time);
				void createAroundLight();
				ci::ColorA mDefaultcolor;
				Utility::softptr<Game::Light::cPointLightParam>light;
				std::vector<Utility::softptr<Game::Light::cPointLightParam>>mAroundLights;
				float lightsinrotate = 0.0f;
				std::vector<float>mAroundLightAngle;
				std::vector<float>mAroundLightColorH;
				float mAroundLightLength;

			private:////見た目の大きさ関連
				void updateScale(const float& delta_time);
				ci::vec3 mDrawscale;
				float mDrawScaleSinAngle = 0.0f;

			private:///爆発関連
				void exprosion();
				void collisonToPlayer();
				bool mIsExprosion = false;
				float mExprosionLength;
			public:
				void setup() override;
				void update(const float& delta_time) override;
				void updateCollisionAfterUpdate(const float& delta_time) override;
				void draw() override;
				bool deleteThis() override;
			private:
				
			};
		};
	};
}