#include <Game/Weapons/SubWeapon/cLightBomb.h>
#include"Game\cPlayerManager.h"
#include<Game/cStrategyManager.h>
#include<Game/cFieldManager.h>
#include<Particle/cParticleManager.h>
#include"Resource\cSoundManager.h"
#include <Game/cClientAdapter.h>
#include"Sound/Wav.h"
#include"Sound\Stereophonic.h"
#include"Resource\cObjectManager.h"
namespace Game
{
namespace Weapons
{
	namespace SubWeapon {
		cLightBomb::cLightBomb(const ci::vec3 pos, const ci::vec3 scale, const ci::vec3 speed, const int playerid,const int objectid)
			: aabb(pos, scale)
			, rb(aabb) {
			mPos = pos;
			mScale = scale;
			mDrawscale = mScale/2.f;
			mObjectId = objectid;
			mSpeed = speed;
			mPlayerId = playerid;
			mTeamNum = cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getWhichTeam();
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
			//Sound::StereophonicManager::getInstance()->deleteMap(mPos);
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
			light->reAttachPositionWithRadius(mPos, (mScale.x + 1.f) + (mScale.x+0.5f)*cos(lightsinrotate));

			light->color = ci::vec3(mTeamNum == 0, 0.5f + 0.5f*cos(lightsinrotate+M_PI), mTeamNum == 1);

			if (mIsHitObject) {
				lightsinrotate += mLandcount;

				for (int i = 0; i < int(mAroundLineLight.size()); i++) {
					mAroundLineLight[i]->reAttachLine(mPos + ci::vec3(mAroundLightLength*cos(mAroundLightAngle[i]),
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

				float gain = 0.6f - glm::distance2(cPlayerManager::getInstance()->getActivePlayer()->getPos(),
					cPlayerManager::getInstance()->getPlayers()[mPlayerId]->getPos()) / 10.f;
				if (gain <= 0.0f) {
					gain = 0.0f;
				}
			/*	Resource::cSoundManager::getInstance()->findSe("SubWeapon/bombexprotion.wav").setGain(gain);
				Resource::cSoundManager::getInstance()->findSe("SubWeapon/bombexprotion.wav").play();*/
				
				Sound::StereophonicManager::getInstance()->add(Sound::Wav(ci::app::getAssetPath("SE/SubWeapon/bombexprotion2.wav").string()), mPos);

				Particle::cParticleManager::getInstance()->create(Particle::ParticleParam().position(mPos)
					.scale(0.5f).
					vanishTime(0.7f).
					speed(0.85f).
					textureType(Particle::ParticleTextureType::SPARK).
					color(ci::ColorA::white()).
					moveType(Particle::ParticleType::EXPROTION).count(30).isTrajectory(true).gravity(0.048f));

			}
		}
		float cLightBomb::getDamage(const float distance, const float maxdamage)
		{
			float damagerate = ((mExprosionLength*2.f - distance) / mExprosionLength);
			if (damagerate > 1.0f)damagerate = 1.0f;
			return damagerate * maxdamage;
		}
		void cLightBomb::createContractionEffect()
		{
			if (mIsContraction)return;
			if (mLandcount >= 2.0f) {
				mIsContraction = true;
				//Resource::cSoundManager::getInstance()->findSe("SubWeapon/frontexprotion.wav").setGain(0.4f);
				//Resource::cSoundManager::getInstance()->findSe("SubWeapon/frontexprotion.wav").play();
				Sound::StereophonicManager::getInstance()->add(Sound::Wav(ci::app::getAssetPath("SE/SubWeapon/frontexprotion2.wav").string()), mPos);
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
			mesh = Resource::cObjectManager::getInstance()->findObject("sphere.obj");
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
			ci::gl::pushModelView();
			ci::gl::translate(mPos);
			ci::gl::color(ci::ColorA(light->color.r, light->color.g, light->color.b));
			ci::gl::scale(mDrawscale);
			ci::gl::draw(mesh);
			ci::gl::popModelView();
			//STRM->drawShere(mPos, mDrawscale, ci::vec3(0, 0, 0), ci::ColorA(light->color.r, light->color.g, light->color.b), 100);
		}
		bool cLightBomb::deleteThis()
		{
			return mIsExprosion;
		}
		void cLightBomb::collisonToPlayer()
		{
			for (int i = 0; i < cPlayerManager::getInstance()->getPlayers().size(); i++) {
				if (cPlayerManager::getInstance()->getPlayers()[i]->getWhichTeam() == mTeamNum)continue;

				float distance = glm::distance2(cPlayerManager::getInstance()->getPlayers()[i]->getPos(), mPos);

				if (distance < mExprosionLength*2.f) {
					cClientAdapter::getInstance()->sendDamage(cPlayerManager::getInstance()->getPlayers()[i]->getPlayerId(),
						getDamage(distance, 100.f));
				}


			}
			
		}
	}
}
}