#include <cinder/app/App.h>
#include <cinder/app/RendererGl.h>
#include <cinder/gl/gl.h>
#include <cinder/Timeline.h>
#include <Utility/cInput.h>
#include <Scene/Member/Tutorial.h>
#include <Game/cFieldManager.h>
#include <Game/cStrategyManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cGemManager.h>
#include <Collision/cCollisionManager.h>
#include <Network.hpp>
#include <Game/cClientAdapter.h>
#include <Game/cServerAdapter.h>
#include <Resource/TextureManager.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <Network/cMatchingMemberManager.h>
#include <Game/cShaderManager.h>
#include <Game/cDebugManager.h>
#include <Game/cLightManager.h>
#include <Game/cUIManager.h>
#include <Particle/cParticleManager.h>
#include<Game/cCapsuleManager.h>
#include<Game/cSubWeaponManager.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>
#include <Game/cTutorialManager.h>
#include <Game/Field/FieldData.h>
#include <Sound/Stereophonic.h>
#include <Resource/cFbxManager.h>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cTitle.h>
#include <Resource/cSoundManager.h>
#include <Game/cGameManager.h>
static ci::vec3 testSoundPos;

using namespace ci;
using namespace ci::app;
using namespace std;
namespace Scene
{
	namespace Member
	{
		void cTutorial::setup()
		{
			Game::cUIManager::getInstance()->awake();
			Game::cDebugManager::getInstance()->setup();
			Sound::StereophonicManager::getInstance()->open();
			Resource::cFbxManager::getInstance()->setup();

			skydome.setup();
			CAMERA->setup();
			Game::cFieldManager::getInstance()->setup();
			Game::cStrategyManager::getInstance()->setup();

			const int active_player_id = Network::cMatchingMemberManager::getInstance()->mPlayerID;

			// チーム決め
			std::vector<int> teams;
			teams.emplace_back(Game::Player::Red);
			teams.emplace_back(Game::Player::Red);
			teams.emplace_back(Game::Player::Red);
			teams.emplace_back(Game::Player::Red);
			teams.emplace_back(Game::Player::Blue);
			teams.emplace_back(Game::Player::Blue);
			teams.emplace_back(Game::Player::Blue);
			teams.emplace_back(Game::Player::Blue);
			std::vector<std::string> names;
			names.emplace_back(u8"名無しもぐ");
			names.emplace_back(u8"名無しもぐ");
			names.emplace_back(u8"名無しもぐ");
			names.emplace_back(u8"名無しもぐ");
			names.emplace_back(u8"名無しもぐ");
			names.emplace_back(u8"名無しもぐ");
			names.emplace_back(u8"名無しもぐ");
			names.emplace_back(u8"名無しもぐ");
			{
				int i = 0;
				for (auto& o :
					Network::cMatchingMemberManager::getInstance()->mNameStrs)
				{
					names[i] = o;
					if (i == 2)
					{
						i += 1;
					}
					i += 1;
				}
			}
			for (auto& o : Network::cMatchingMemberManager::getInstance()->mPlayerDatas)
			{
				teams[o.playerID] = o.teamNum;
				//names[o.playerID] = o.nameStr;
			}
			teams[active_player_id] = Network::cMatchingMemberManager::getInstance()->mPlayerTeamNum;
			//if (Network::cMatchingMemberManager::getInstance()->mPlayerName != "moguraNull")
			//{
			//	names[active_player_id] = Network::cMatchingMemberManager::getInstance()->mPlayerName;
			//}

			Game::cPlayerManager::getInstance()->setup(Game::Field::RESPAWN_POINT, 8U, active_player_id, teams, names);
			// ////////プレイヤーが揃った後に、キャノンにはプレイヤーの使用している全ライトを適応させる。
			Game::cStrategyManager::getInstance()->lightSetup();

			int seed = 20171031;
			GemManager->setUp(vec3(0, 0, 0),
				vec3(Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_X / 2,
					Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_Y,
					Game::Field::CHUNK_SIZE * Game::Field::CHUNK_RANGE_Z), Game::Field::BLOCK_SIZE, 0.5, 100, seed);
			Collision::cCollisionManager::getInstance()->setup();
			Game::cLightManager::getInstance()->setup();
			Game::cShaderManager::getInstance()->setup();
			Game::cCapsuleManager::getInstance()->setup();
			Game::cSubWeaponManager::getInstance()->setup();
			Game::cUIManager::getInstance()->setup();
			Resource::cFbxManager::getInstance()->setup();

			sendEndSetup = false;
			gl::enableDepthRead();
			gl::enableDepthWrite();

			ENV->disableKeyButton();
			ENV->disablePadButton();
			ENV->disableMouseButton();
			ENV->disablePadAxis();

			Game::cUIManager::getInstance()->disable();
		}

		void cTutorial::shutDown()
		{
			Resource::cSoundManager::getInstance()->findBgm("トロピカル無職.wav").stop();
			Network::cUDPClientManager::removeInstance();
			Network::cUDPServerManager::removeInstance();
			Game::cDebugManager::removeInstance();
			Game::cClientAdapter::removeInstance();
			Game::cServerAdapter::removeInstance();
			Game::cUIManager::removeInstance();
			Game::cCapsuleManager::removeInstance();
			// サブウェポンの一つクァーリーは死ぬ時にプレイヤーマネージャーとストラテジーマネージャーに
			// 依存しているのでそれより前。
			Game::cSubWeaponManager::removeInstance();
			Game::cStrategyManager::removeInstance();
			Game::cPlayerManager::removeInstance();
			Game::cShaderManager::removeInstance();
			Particle::cParticleManager::removeInstance();
			Game::cGemManager::removeInstance();
			Game::cGameManager::removeInstance();
			Game::cTutorialManager::removeInstance();
			// 全てのマネージャーのライトを削除するためライトを使っているマネージャーより下。
			Game::cLightManager::removeInstance();
			// ライトを削除する時にチャンクIDを取得しているためライトより下。
			Game::cFieldManager::getInstance()->shutdown();
			Game::cFieldManager::removeInstance();
			// コライダーの削除に耐えるためフィールドより下。
			Collision::cCollisionManager::removeInstance();
			//立体音響の削除
			Sound::StereophonicManager::getInstance()->clear();
			Sound::StereophonicManager::getInstance()->close();
			c_Easing::clear();
			Game::cTutorialManager::removeInstance();
		}

		void cTutorial::update(float deltaTime)
		{
			Network::cUDPClientManager::getInstance()->update(deltaTime);
			Network::cUDPServerManager::getInstance()->update(deltaTime);
			if (Network::cUDPClientManager::getInstance()->isConnected())
			{
				if (sendEndSetup == false)
				{
					Network::cUDPClientManager::getInstance()->send(new Network::Packet::Request::cReqEndGamemainSetup());
					sendEndSetup = true;
				}

				// 他のアップデートよりも先に行います。
				Game::cTutorialManager::getInstance()->preUpdate(deltaTime);
				Game::cTutorialManager::getInstance()->update(deltaTime);
				Game::cDebugManager::getInstance()->update(deltaTime);
				Game::cClientAdapter::getInstance()->update();
				Game::cServerAdapter::getInstance()->update();
				Sound::StereophonicManager::getInstance()->update(deltaTime);
				Game::cUIManager::getInstance()->update(deltaTime);
				Game::cFieldManager::getInstance()->update(deltaTime);
				if (!Game::cTutorialManager::getInstance()->getTutorialStan()) {
					Game::cPlayerManager::getInstance()->update(deltaTime);
				}
				Game::cStrategyManager::getInstance()->update(deltaTime);
				Game::cCapsuleManager::getInstance()->update(deltaTime);
				Game::cSubWeaponManager::getInstance()->update(deltaTime);
				Collision::cCollisionManager::getInstance()->update(deltaTime);
				if (!Game::cTutorialManager::getInstance()->getTutorialStan()) {
					Game::cPlayerManager::getInstance()->playerCollisionAfterUpdate(deltaTime);
				}
				Game::cSubWeaponManager::getInstance()->updateCollisionAfterUpdate(deltaTime);
				GemManager->update(deltaTime);
				Game::cLightManager::getInstance()->update();
				Game::cShaderManager::getInstance()->update(std::bind(&cTutorial::drawShadow, this));
				Particle::cParticleManager::getInstance()->update(deltaTime);
				GemManager->lateUpdate(deltaTime);
				Resource::cFbxManager::getInstance()->testUpdate(deltaTime);
				if (ENV->pushKey(cinder::app::KeyEvent::KEY_F5))
				{
					cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
				}
			}
		}

		void cTutorial::draw()
		{
			Game::cShaderManager::getInstance()->draw([this]
			{
				ci::gl::ScopedColor scpCol(ColorA(1.0F, 1.0F, 1.0F, 1.0F));

				gl::enableDepthRead();
				gl::enableDepthWrite();
				Game::cFieldManager::getInstance()->draw();
				Game::cShaderManager::getInstance()->uniformUpdate();
				Game::cStrategyManager::getInstance()->draw();
				Game::cSubWeaponManager::getInstance()->draw();
				Game::cCapsuleManager::getInstance()->draw();
				GemManager->draw();
				skydome.draw();
				CAMERA->unBind3D();

				CAMERA->bind2D();
				gl::disableDepthRead();
				gl::disableDepthWrite();
				GemManager->drawFbo();
				CAMERA->unBind2D();

				CAMERA->bind3D();
				gl::enableDepthRead();
				gl::enableDepthWrite();
				Game::cPlayerManager::getInstance()->draw();
				Resource::cFbxManager::getInstance()->draw("mogura");

				Particle::cParticleManager::getInstance()->draw();
			});

			Collision::cCollisionManager::getInstance()->draw();
		}

		void cTutorial::drawShadow()
		{
			ci::gl::ScopedColor scpCol(ColorA(1.0F, 1.0F, 1.0F, 1.0F));
			gl::enableDepthRead();
			gl::enableDepthWrite();
			Game::cFieldManager::getInstance()->draw();
			Game::cStrategyManager::getInstance()->draw();
			Game::cPlayerManager::getInstance()->draw();
		}

		void cTutorial::draw2D()
		{
			gl::enableFaceCulling(false);
			gl::disableDepthRead();
			gl::disableDepthWrite();
			ci::gl::pushModelView();
			Game::cUIManager::getInstance()->draw();
			Game::cDebugManager::getInstance()->draw2d();
			ci::gl::popModelView();
			Game::cTutorialManager::getInstance()->draw();
		}

		void cTutorial::resize()
		{

		}


	}
}