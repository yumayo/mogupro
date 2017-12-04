#include <Game/Weapons/SubWeapon/cLightBomb.h>
#include"Game\cPlayerManager.h"
#include<Game/cStrategyManager.h>
#include<Game/cFieldManager.h>
#include<Particle/cParticleManager.h>
#include"Resource\cSoundManager.h"
#include <Game/cClientAdapter.h>
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
			ci::app::console() << "playerID"<<mPlayerId << std::endl;
			mPlayerId = playerid;
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

			for (int i = 0; i < mAroundLineLight.size(); i++) {
				cLightManager::getInstance()->removeLineLight(mAroundLineLight[i]);
			}
		}

		void cLightBomb::dmageToPlayer(const int playerid)
		{
			cClientAdapter::getInstance( )->sendDamage( playerid, 100.0F );
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

				for (int i = 0; i < int(mAroundLineLight.size()); i++) {
					mAroundLineLight[i]->reAttachLine(mAroundLineLight[i], mPos + ci::vec3(mAroundLightLength*cos(mAroundLightAngle[i]),
						-0.1f, mAroundLightLength*sin(mAroundLightAngle[i])),
						mPos + ci::vec3(mAroundLightLength*cos(mAroundLightAngle[i]+2.f*M_PI / 3.f),
							-0.1f, mAroundLightLength*sin(mAroundLightAngle[i] + 2.f*M_PI / 3.f)));

					mAroundLightColorH[i] += delta_time;
					float h = std::fmodf(mAroundLightColorH[i], 1.0f);
					ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.0f));
					mAroundLineLight[i]->color = ci::vec3(hsv.r, hsv.g, hsv.b);
					mAroundLightAngle[i] += mLandcount/10.f;
				}
			}
			else {
				light->color = ci::vec3(mDefaultcolor.r, mDefaultcolor.g, mDefaultcolor.b);
			}
		}
		void cLightBomb::createAroundLight()
		{
			const float createnum = 20.f;

			for (float i = 0; i < 3; i++) {
				float angle = (i / 3.f)*2.f*M_PI;
				mAroundLightAngle.emplace_back(angle);
				mAroundLightColorH.emplace_back(i / 3.f);
				ci::vec3 beginpos = mPos + ci::vec3(mAroundLightLength*cos(angle), 0, mAroundLightLength*sin(angle));
				ci::vec3 endpos = mPos + ci::vec3(mAroundLightLength*cos(angle+2.f*M_PI/3.f), 0, mAroundLightLength*sin(angle+2.f*M_PI / 3.f));
				float h = (i / 3.f);
				ci::Colorf hsv = ci::hsvToRgb(ci::vec3(h, 1.0f, 1.f));
				mAroundLineLight.emplace_back(cLightManager::getInstance()->addLineLight(beginpos, endpos,ci::vec3(hsv.r, hsv.g, hsv.b), 1.0f));
			}


		}
		void cLightBomb::updateScale(const float & delta_time)
		{
			mDrawScaleSinAngle += mLandcount*mLandcount*delta_time*5.f;
			float scale = mScale.x/2.f + (0.25f*mScale.x)*sin((mDrawScaleSinAngle));

			mDrawscale = ci::vec3(scale);
		}
		void cLightBomb::exprosion()
		{
			if (mIsExprosion)return;
			float exprosiontime = 3.0f;

			if (mLandcount >= 3.0f) {
				////////アクティブプレイヤーのみ行います
				if (mPlayerId == cPlayerManager::getInstance()->getActivePlayerId()) {
					collisonToPlayer();

					Game::cFieldManager::getInstance()->blockBreak(mPos, mExprosionLength / 2.f);
				}
				mIsExprosion = true;

				Resource::cSoundManager::getInstance()->findSe("SubWeapon/bombexprotion.wav").setGain(0.6f);
				Resource::cSoundManager::getInstance()->findSe("SubWeapon/bombexprotion.wav").play();
				


				Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
					.scale(0.5f).
					vanishTime(1.0f).
					speed(1.0f).
					textureType(Particle::ParticleTextureType::SPARK).
					color(ci::ColorA::white()).
					moveType(Particle::ParticleType::EXPROTION).count(100).isTrajectory(true).gravity(0.048f));

			}
		}
		void cLightBomb::createContractionEffect()
		{
			if (mIsContraction)return;
			if (mLandcount >= 2.0f) {
				mIsContraction = true;
				Resource::cSoundManager::getInstance()->findSe("SubWeapon/frontexprotion.wav").setGain(0.4f);
				Resource::cSoundManager::getInstance()->findSe("SubWeapon/frontexprotion.wav").play();
				Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
					.scale(1.3f).vanishBeginTime(0.f).vanishTime(24.f/60.f).vanishTimeRange(0.0f).
					easeTime(24.f).
					speed(0.0f).
					textureType(Particle::ParticleTextureType::SPARK).
					color(ci::ColorA(1,1,0)).
					moveType(Particle::ParticleType::CONVERGE).count(10).isTrajectory(true).effectTime(0.6f).easeType(EaseType::BackIn));
			}
		}
		void cLightBomb::setup()
		{
			aabb.addWorld();
			rb.addWorld();
			rb.setSpeed(mSpeed);
			rb.setFriction(1.0f);
			
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
			createContractionEffect();
			exprosion();
		
		}
		void cLightBomb::updateCollisionAfterUpdate(const float & delta_time)
		{
			mPos = aabb.getPosition();
			mSpeed = rb.getSpeed();
		}
		void cLightBomb::draw()
		{
			STRM->drawShere(mPos, mDrawscale, ci::vec3(0, 0, 0), ci::ColorA(light->color.r, light->color.g, light->color.b), 100);
		}
		bool cLightBomb::deleteThis()
		{
			return mIsExprosion;
		}
		void cLightBomb::collisonToPlayer()
		{
			for (int i = 0; i < cPlayerManager::getInstance()->getPlayers().size(); i++) {
				if (cPlayerManager::getInstance()->getPlayers()[i]->getWhichTeam() == mTeamNum)continue;
				if (glm::distance2(cPlayerManager::getInstance()->getPlayers()[i]->getPos(), mPos) < mExprosionLength) {
					dmageToPlayer(i);
				}


			}
			
		}
	}
}
}