#include <Game/Weapons/SubWeapon/cLightBomb.h>
#include"Game\cPlayerManager.h"
#include<Game/cStrategyManager.h>
#include<Game/cFieldManager.h>
#include<Particle/cParticleManager.h>
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
			mDrawscale = mScale/2.f;
			mSpeed = speed;
			mPlayerId = playerid;
			mTeamNum = cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getWhichTeam();
			
			if (mTeamNum == 0) {
				mDefaultcolor = ci::ColorA(1, 0, 0, 1);
			}
			else {
				mDefaultcolor = ci::ColorA(0, 0, 1, 1);
			}
			
		}
		cLightBomb::~cLightBomb()
		{
			aabb.removeWorld();
			rb.removeWorld();
			cLightManager::getInstance()->removePointLight(light);

			for (int i = 0; i < mAroundLights.size(); i++) {
				cLightManager::getInstance()->removePointLight(mAroundLights[i]);
			}

		}

		void cLightBomb::dmageToPlayer(const int playerid)
		{
			//cPlayerManager::getInstance()->getPlayers()[playerid]
		}

		void cLightBomb::hitObject()
		{
			if (!mIsHitObject) {
				if (rb.isLanding()) {
					mIsHitObject = true;
					createAroundLight();
				}
			}
		}
		void cLightBomb::updateLight(const float & delta_time)
		{
			light->reAttachPositionWithRadius(light, mPos, (mScale.x + 1.f) + (mScale.x+0.5f)*cos(lightsinrotate));

			light->color = ci::vec3(mTeamNum == 0, 0.5f + 0.5f*cos(lightsinrotate+M_PI), mTeamNum == 1);

			if (mIsHitObject) {
				lightsinrotate += mLandcount;
				for (int i = 0; i < mAroundLights.size(); i++) {
					mAroundLights[i]->reAttachPosition(mAroundLights[i], mPos + ci::vec3(mAroundLightLength*cos(mAroundLightAngle[i]),
						0, mAroundLightLength*sin(mAroundLightAngle[i])));

					mAroundLightColorH[i] += delta_time;
					float h = std::fmodf(mAroundLightColorH[i], 1.0f);
					ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.0f));
					mAroundLights[i]->color = ci::vec3(hsv.r, hsv.g, hsv.b);
					//mAroundLightAngle[i] += mLandcount/10.f;
				}
			}
			else {
				light->color = ci::vec3(mDefaultcolor.r, mDefaultcolor.g, mDefaultcolor.b);
			}
		}
		void cLightBomb::createAroundLight()
		{
			const float createnum = 20.f;

			for (float i = 0.0f; i < createnum; i++) {
				float angle = (i / createnum)*2.f*M_PI;
				mAroundLightAngle.push_back(angle);
				mAroundLightColorH.push_back(i / createnum);
				ci::vec3 pos = mPos + ci::vec3(mAroundLightLength*cos(angle), 0, mAroundLightLength*sin(angle));
				float h = (i / createnum);
				ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.f));

				mAroundLights.push_back(cLightManager::getInstance()->addPointLight(pos, ci::vec3(hsv.r, hsv.g, hsv.b), 0.5f));
			}
		}
		void cLightBomb::updateScale(const float & delta_time)
		{
			mDrawScaleSinAngle += mLandcount*mLandcount*delta_time*5.f;
			float scale = mScale.x/2.f + (0.25f*mScale.x)*sin((mDrawScaleSinAngle));

			mDrawscale = ci::vec3(scale, scale, scale);
		}
		void cLightBomb::exprosion()
		{
			if (mIsExprosion)return;
			if (mLandcount >= 3.0f) {
				collisonToPlayer();
				Game::cFieldManager::getInstance()->blockBreak(mPos, mExprosionLength/2.f);
				mIsExprosion = true;
				//Particle::cParticleManager::getInstance()->create(mPos, Particle::ParticleType::SCATTER, Particle::ParticleTextureType::NONE, 1.0f);
			}
		}
		void cLightBomb::setup()
		{
			aabb.addWorld();
			rb.addWorld();
			rb.setSpeed(mSpeed);
			rb.setFriction(1.0f);
			ci::app::console() << "‚¤‚Ü‚ê‚½" << std::endl;
			light = cLightManager::getInstance()->addPointLight(mPos, ci::vec3(0, 1, 0), 1.f);
			mExprosionLength = 8.f*mScale.x;
			mAroundLightLength = (mExprosionLength / 2.f) - 0.5f;
		
		}
		void cLightBomb::update(const float & delta_time)
		{
			hitObject();
			if (mIsHitObject) {
				mLandcount += delta_time;
			}
			updateLight(delta_time);
			updateScale(delta_time);
			exprosion();
		}
		void cLightBomb::updateCollisionAfterUpdate(const float & delta_time)
		{
			mPos = aabb.getPosition();
			mSpeed = rb.getSpeed();
		}
		void cLightBomb::draw()
		{
			STRM->drawShere(mPos, mDrawscale, ci::vec3(0, 0, 0), ci::ColorA(light->color.r, light->color.g, light->color.b), 20);
		}
		bool cLightBomb::deleteThis()
		{
			return mIsExprosion;
		}
		void cLightBomb::collisonToPlayer()
		{
			for (int i = 0; i < cPlayerManager::getInstance()->getPlayers().size(); i++) {
				//if (cPlayerManager::getInstance()->getPlayers()[i]->getWhichTeam() == mTeamNum)continue;
				if (glm::distance2(cPlayerManager::getInstance()->getPlayers()[i]->getPos(), mPos) < mExprosionLength) {
					//dmageToPlayer(i);
					ci::app::console() << "‚ ‚Á‚½‚Á‚½" << std::endl;
				}


			}
			
		}
	}
}
}