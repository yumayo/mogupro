#include <Game/Weapons/SubWeapon/cLightBomb.h>
#include"Game\cPlayerManager.h"
#include<Game/cStrategyManager.h>
namespace Game
{
namespace Weapons
{
	namespace SubWeapon {
		cLightBomb::cLightBomb(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 speed, const int playerid)
			: aabb(pos, scale)
			, rb(aabb) {
			mPos = pos;
			mScale = scale;
			mSpeed = speed;
			mPlayerId = playerid;
		}
		cLightBomb::~cLightBomb()
		{
			aabb.removeWorld();
			rb.removeWorld();
			
		}
		void cLightBomb::setup()
		{
			aabb.addWorld();
			rb.addWorld();
			rb.setSpeed(mSpeed);
			ci::app::console() << "‚¤‚Ü‚ê‚½" << std::endl;
			light = cLightManager::getInstance()->addPointLight(mPos, ci::vec3(0, 1, 0), 1.f);
			
		}
		void cLightBomb::update(const float & delta_time)
		{
			//mPos += mSpeed;
			light->reAttachPositionWithRadius(light,mPos, 2.f + 1.f*cos(lightsinrotate));
			light->color = ci::vec3(1, 0.5f + 0.5f*cos(lightsinrotate), 0);
			lightsinrotate += delta_time*5.f;
			if ((!is)&& lightsinrotate > 10.0f) {
				cLightManager::getInstance()->removePointLight(light);
				is = true;
			}
		}
		void cLightBomb::updateCollisionAfterUpdate(const float & delta_time)
		{
			mPos = aabb.getPosition();
			mSpeed = rb.getSpeed();
		}
		void cLightBomb::draw()
		{
			STRM->drawCube(mPos, mScale, ci::vec3(0, 0, 0), ci::ColorA(1, 0, 0, 1));
		}
		bool cLightBomb::deleteThis()
		{
			return lightsinrotate > 50.0f;
		}
	}
}
}