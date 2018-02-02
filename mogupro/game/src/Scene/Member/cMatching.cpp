#include <Scene/Member/cMatching.h>
#include "cinder/gl/gl.h"
#include <Utility/cInput.h>
#include <string>
#include <Network.hpp>
#include <Scene/Member/cGameMain.h>
#include <Scene/cSceneManager.h>
#include <Network/cMatchingMemberManager.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include "CameraManager\cCameraManager.h"
#include <Resource/TextureManager.h>
#include <Resource/cJsonManager.h>
#include <Resource/cImageManager.h>
#include <Log/Log.h>
#include <cinder/Rand.h>

using namespace Network;
using namespace Network::Packet::Event;
using namespace Network::Packet::Request;
using namespace Network::Packet::Response;
using namespace Node::Action;

DrillUI::DrillUI() { }

DrillUI::DrillUI(ci::vec2 pos, ci::vec2 moveVec, std::string name)
{
	using namespace Node;
	using namespace Node::Action;
	mRoot = Node::node::create();
	mRoot->set_schedule_update();
	mRoot->set_position(pos);
	mRoot->run_action(ease<ci::EaseOutCirc>::create(
		move_to::create(3.0F, ci::vec3(moveVec.x, moveVec.y, 0))));
	auto plate = Node::Renderer::sprite::create(Resource::IMAGE["matching/drillUI2.png"]);
	plate->set_position(ci::vec2(0, 0));
	plate->set_scale(glm::vec2(1.8f, 0.8f));
	mRoot->add_child(plate);

	auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
	f->set_text(u8"" + name);
	f->set_position(ci::vec2(0, 0));
	f->set_schedule_update();
	f->set_scale(glm::vec2(1, -1));
	mRoot->add_child(f);
}

void DrillUI::update(float deltaTime)
{
	mRoot->entry_update(deltaTime);
}

void DrillUI::draw()
{
	mRoot->entry_render(cinder::mat4());
}

Star::Star()
{
	time = ci::randFloat(0.0f, 1.0f);
	value = ci::randFloat(1.4f,-1.4f);

	using namespace Node;
	using namespace Node::Action;
	mRoot = Node::node::create();
	mRoot->set_schedule_update();
	mRoot->set_position(ci::vec2(ci::randFloat(-800,800), ci::randFloat(-450,450)));

	auto star = Node::Renderer::sprite::create(Resource::IMAGE["matching/star.png"]);
	star->set_schedule_update();
	star->set_tag(0);
	star->set_position(ci::vec2(0, 0));
	mRoot->add_child(star);

	mRoot->get_child_by_tag(0)->run_action(
		repeat_forever::create(
			sequence::create(
				ease<ci::EaseInOutCirc>::create(scale_by::create(time, ci::vec2(-value))),
				ease<ci::EaseInOutCirc>::create(scale_by::create(time, ci::vec2(value)))
			)
		)
	);

}
void Star::update(float deltaTime)
{
	mRoot->entry_update(deltaTime);
}
void Star::draw()
{
	mRoot->entry_render(cinder::mat4());
}

namespace Scene
{
	namespace Member
	{
		void cMatching::setup()
		{
			Network::cUDPClientManager::getInstance()->open();
			cUDPClientManager::getInstance()->connect( Resource::JSON["server.json"]["ip"].asString( ) );
			mClassState = ClassState::NOT;
			mWaitClassState = ClassState::NOT;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mCanSend = true;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mSelectTag = 0;
			mPrevSelectTag = 0;
			mTeamNum = -1;
			mBeginAnimation = false;
			registerFunc();
			mTrimeshAnimationFbo = ci::gl::Fbo::create(ci::app::getWindowWidth(),
				ci::app::getWindowHeight(), true);
			sceneChange = false;
			teamCount[0] = 0;
			teamCount[1] = 0;
			Log::cLogManager::getInstance()->add("Matching");
			Log::cLogManager::getInstance()->add("TeamNum");

			for(int i = 0; i < 50;++i)
			stars.push_back(Star());
		}

		void cMatching::registerFunc()
		{
			mRoot = Node::node::create();
			mRoot->set_schedule_update();
			auto backView = Node::Renderer::sprite::create(Resource::IMAGE["matching/nightSky.png"]);
			backView->set_position(ci::vec2(0, 0));
			backView->set_scale(ci::vec2(2.0f, -1.5f));
			mRoot->add_child(backView);
			
			//対戦ボタンUI
			auto fightPlate = Node::Renderer::sprite::create(Resource::IMAGE["matching/greenUI.png"]);
			fightPlate->set_schedule_update();
			fightPlate->set_tag(0);
			fightPlate->set_position(ci::vec2(-250, 0));
			mRoot->add_child(fightPlate);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf",50);
				f->set_text(u8"対戦");
				f->set_scale(glm::vec2(1, -1));
				fightPlate->add_child(f);
			}
			outRoomFunc.emplace_back(
				[this] {
				mCanSend = false;
				cUDPClientManager::getInstance()->send(new cReqMakeRoom(100));
				mWaitClassState = ClassState::MASTER;
			});

			//観戦ボタンUI
			auto lookPlate = Node::Renderer::sprite::create(Resource::IMAGE["matching/redUI.png"]);
			lookPlate->set_schedule_update();
			lookPlate->set_tag(1);
			lookPlate->set_position(ci::vec2(250, 0));
			mRoot->add_child(lookPlate);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 50);
				f->set_text(u8"観戦");
				f->set_scale(glm::vec2(1, -1));
				lookPlate->add_child(f);
			}
			outRoomFunc.emplace_back(
				[this] {
				// とりあえずコントロールを押しながらで観戦者。
				//if ( ENV->pressKey( cinder::app::KeyEvent::KEY_LCTRL ) )
				cUDPClientManager::getInstance( )->send( new cReqInRoomWatching( 100 ) );
				mCanSend = false;
				mWaitClassState = ClassState::CLIENT;
				mSelectTag = 0;
			});
			mRoot->get_child_by_tag(mSelectTag)->run_action(
				repeat_forever::create(
					sequence::create(
						ease<ci::EaseInOutCirc>::create(scale_by::create(0.26F, ci::vec2(0.2F))),
						ease<ci::EaseInOutCirc>::create(scale_by::create(0.26F, ci::vec2(-0.2F)))
					)
				)
			);

			inRoomFunc.emplace_back(
				[this] {
				if (mClassState == ClassState::MASTER)
				{
					cUDPClientManager::getInstance()->send(new cReqCheckBeginGame());
					mCanSend = false;
				}
			});

			mMemberRoot = Node::node::create();
			mMemberRoot->set_scale(ci::vec2(1, 1));
			mMemberRoot->set_schedule_update();
			{
				auto lookPlate = Node::Renderer::sprite::create(Resource::IMAGE["matching/eastPlate.png"]);
				lookPlate->set_position(ci::vec2(-350, 285));
				mMemberRoot->add_child(lookPlate);

				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 50);
				f->set_color(ci::ColorA(1, 0, 0));
				f->set_text(u8"赤軍");
				f->set_scale(glm::vec2(1, -1));
				lookPlate->add_child(f);
			}
			{
				auto plate = Node::Renderer::sprite::create(Resource::IMAGE["matching/eastPlate.png"]);
				plate->set_position(ci::vec2(350, 285));
				mMemberRoot->add_child(plate);

				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 50);
				f->set_color(ci::ColorA(0, 0, 1));
				f->set_text(u8"青軍");
				f->set_scale(glm::vec2(1, -1));
				plate->add_child(f);
			}
		}
		void cMatching::setAnimation()
		{
			auto m = Node::Renderer::rect::create(ci::vec2(100, 100));
			m->set_position(ci::vec2(0, 0));
			m->set_color(ci::ColorA(1, 0, 0));
			m->set_schedule_update();
			m->set_axis(ci::vec3(1, 1, 0));
			m->run_action(sequence::create(spawn::create(ease<ci::EaseInOutCirc>::create(scale_by::create(3.0f, ci::vec2(2.2f))),
				rotate_to::create(2.0F, M_PI * 2)),
				fade_out::create(1.5f),
				call_func::create([this] {
				for (auto& m : drillUI1Ps)
				{
					ci::vec2 pos = m.mRoot->get_position();
					m.mRoot->run_action(sequence::create(move_to::create(3.5F, ci::vec3(-1000, pos.y, 0)),
						ease<ci::EaseOutCirc>::create(move_to::create(4.0F, ci::vec3(-200, pos.y, 0)))));
				}

				for (auto& m : drillUI2Ps)
				{
					ci::vec2 pos = m.mRoot->get_position();
					m.mRoot->run_action(sequence::create(move_to::create(3.5F, ci::vec3(1000, pos.y, 0)),
						ease<ci::EaseOutCirc>::create(move_to::create(4.0F, ci::vec3(200, pos.y, 0)))));
				}
			}),
				delay::create(5.0f),
				call_func::create([this]
			{
				ci::gl::ScopedFramebuffer fbScp(mTrimeshAnimationFbo);
				ci::gl::ScopedViewport vp(ci::ivec2(0), mTrimeshAnimationFbo->getSize());
				ci::gl::clear(ci::ColorA(0, 1, 0, 1));
				ci::gl::disableDepthRead();
				ci::gl::disableDepthWrite();
				CAMERA->bind2D();
				ci::gl::color(ci::ColorA(1, 0, 0, 1));
				mRoot->entry_render(cinder::mat4());
				for (auto& m : drillUI1Ps)
					m.draw();
				for (auto& m : drillUI2Ps)
					m.draw();
				mTrimeshAnimation.make(ci::vec2(1600, 900), ci::vec2(20, 15));
				mBeginAnimation = true;
			}),
				delay::create(4.0f), 
				call_func::create([this]
			{
				sceneChange = true;
			})));
			mMemberRoot->add_child(m);
		}

		void cMatching::shutDown()
		{

		}

		void cMatching::update(float deltaTime)
		{
			if (sceneChange == true)return;

			mPrevSelectTag = mSelectTag;
			mRoot->entry_update(deltaTime);
			mMemberRoot->entry_update(deltaTime);
			
			for (auto& m : stars)
				m.update(deltaTime);

			for (auto& m : drillUI1Ps)
				m.update(deltaTime);
			for (auto& m : drillUI2Ps)
				m.update(deltaTime);
			if (mBeginAnimation == true)
				mTrimeshAnimation.update(deltaTime);
			Network::cUDPClientManager::getInstance()->update(deltaTime);
			if (cUDPClientManager::getInstance()->isConnected() == false)return;
			makeRoom();
			inRoom();
			updateBoxFunc();
			if (sceneChange == true)
			{
				cSceneManager::getInstance( )->shift<Scene::Member::cGameMain>( );
			}

		}


		void cMatching::updateBoxFunc()
		{
			if (mPrevSelectTag != mSelectTag)
			{
				mRoot->get_child_by_tag(mPrevSelectTag)->remove_all_actions();
				mRoot->get_child_by_tag(mPrevSelectTag)->set_scale(ci::vec2(1.0F));
				mRoot->get_child_by_tag(mSelectTag)->run_action(
					repeat_forever::create(
						sequence::create(
							ease<ci::EaseInOutCirc>::create(scale_by::create(0.26F, ci::vec2(0.2F))),
							ease<ci::EaseInOutCirc>::create(scale_by::create(0.26F, ci::vec2(-0.2F)))
						)
					)
				);
			}

			if (mPhaseState == PhaseState::IN_ROOM && mClassState == ClassState::CLIENT
				|| mClassState == ClassState::MASTER)
			{
				auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
				while (auto resCheckBeginGame = m->ResCheckBeginGame.get())
				{
					cMatchingMemberManager::getInstance()->mPlayerID = resCheckBeginGame->mPlayerID;
					setAnimation();
					continue;
				}
			}
		}

		void cMatching::makeRoom()
		{
			if (mPhaseState != PhaseState::NOT_IN_ROOM)return;

			if (mWaitClassState == ClassState::NOT)
			{
				auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
				while (auto resMakeRoom = m->ResMakeRoom.get())
				{
					continue;
				}

				if (ENV->pushKey(ci::app::KeyEvent::KEY_RIGHT))
				{
					mSelectTag = std::min(mSelectTag + 1, 1);
				}

				if (ENV->pushKey(ci::app::KeyEvent::KEY_LEFT))
				{
					mSelectTag = std::max(mSelectTag - 1, 0);
				}

				if (ENV->pushKey(ci::app::KeyEvent::KEY_RETURN) && mCanSend)
				{
					outRoomFunc[mSelectTag]();
				}

			}
			else if (mWaitClassState == ClassState::CLIENT)
			{
				auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
				while (auto resInRoom = m->ResInRoom.get())
				{
					if (resInRoom->mFlag = false)
					{
						cUDPClientManager::getInstance()->send(new cReqInRoom(100));
						mWaitClassState = ClassState::CLIENT;
						continue;
					}

					mWaitClassState = ClassState::NOT;
					mCanSend = true;
					mClassState = ClassState::CLIENT;
					mPhaseState = PhaseState::IN_ROOM;
					//とりあえず0でTeamに入る申請
					//結局ServerでTeamはランダムに決める
					cUDPClientManager::getInstance()->send(new cReqWantTeamIn(0));
					mCanSend = false;
					addInRoomUI();
				}
			}

			else if (mWaitClassState == ClassState::MASTER)
			{
				auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
				while (auto resMakeRoom = m->ResMakeRoom.get())
				{
					if (resMakeRoom->mFlag == false)
					{
						cUDPClientManager::getInstance()->send(new cReqInRoom(100));
						mWaitClassState = ClassState::CLIENT;
						continue;
					}

					mWaitClassState = ClassState::NOT;
					mCanSend = true;
					mClassState = ClassState::MASTER;
					mPhaseState = PhaseState::IN_ROOM;
					//とりあえず0でTeamに入る申請
					//結局ServerでTeamはランダムに決める
					cUDPClientManager::getInstance()->send(new cReqWantTeamIn(0));
					mCanSend = false;
					addInRoomUI();
				}
			}
		}

		void cMatching::inRoom()
		{
			if (mPhaseState != PhaseState::IN_ROOM)return;
			mAddMember = false;
			if (mClassState == ClassState::NOT)return;

			if (ENV->pushKey(ci::app::KeyEvent::KEY_RETURN) && mCanSend)
				inRoomFunc[mSelectTag]();

			//Teamに入れたかどうか
			auto m = Network::cUDPClientManager::getInstance( )->getUDPManager( );
			while (auto resWantTeamIn = m->ResWantTeamIn.get())
			{
				//!@ TODO : Add Performance (Team%Dに入れました or Team%Dに入れませんでした)
				if (resWantTeamIn->mFlag == 1)
					mTeamNum = resWantTeamIn->mTeamNum;
				Network::cMatchingMemberManager::getInstance()->mPlayerTeamNum = mTeamNum;
				mCanSend = true;
				if (resWantTeamIn->mTeamNum == 0)
					drillUI1Ps.push_back(DrillUI(ci::vec2(-1000, 200 - 200 * teamCount[0]),
						ci::vec2(-200, 200 - 200 * teamCount[0]), "You"));
				
				else if(resWantTeamIn->mTeamNum == 1)
					drillUI2Ps.push_back(DrillUI(ci::vec2(1000, 200 - 200 * teamCount[1]),
						ci::vec2(200, 200 - 200 * teamCount[1]), "You"));
				teamCount[mTeamNum]++;
			}
			//TODO : 参加した場合とTeamが変更された場合は分けるべき
			int count = 0;
			while (auto eveTeamMember = m->EveTeamMember.get())
			{
				cMatchingMemberManager::getInstance()->addPlayerDatas(
					eveTeamMember->mNameStr, eveTeamMember->mTeamNum, eveTeamMember->mPlayerID, cNetworkHandle("", 0));
				if (eveTeamMember->mTeamNum == 0)
					drillUI1Ps.push_back(DrillUI(ci::vec2(-1000, 200 - 200 * teamCount[0]),
						ci::vec2(-200, 200 - 200 * teamCount[0]), eveTeamMember->mNameStr));
				else if (eveTeamMember->mTeamNum == 1)
					drillUI2Ps.push_back(DrillUI(ci::vec2(1000, 200 - 200 * teamCount[1]),
						ci::vec2(200, 200 - 200 * teamCount[1]), eveTeamMember->mNameStr));
				teamCount[eveTeamMember->mTeamNum]++;
				Log::cLogManager::getInstance()->writeLog("TeamNum", "TeamNum0 : " + std::to_string(teamCount[0]));
				Log::cLogManager::getInstance()->writeLog("TeamNum", "TeamNum1 : " + std::to_string(teamCount[1]));
				++ count;
			}
			
			Log::cLogManager::getInstance()->writeLog("Matching", "Matching : " + std::to_string(count));
		}

		void cMatching::addInRoomUI()
		{
			mRoot->remove_all_children();
			auto backView = Node::Renderer::sprite::create(Resource::IMAGE["matching/nightSky.png"]);
			backView->set_tag(1);
			backView->set_position(ci::vec2(0, 0));
			backView->set_scale(ci::vec2(1.0f, -1.0f));
			mRoot->add_child(backView);
			mSelectTag = 0;

			if (mClassState != ClassState::MASTER)
				return;

			auto startPlate = Node::Renderer::sprite::create(Resource::IMAGE["matching/greenUI.png"]);
			startPlate->set_schedule_update();
			startPlate->set_tag(0);
			startPlate->set_position(ci::vec2(500, -200));
			mRoot->add_child(startPlate);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 42);
				f->set_text(u8"ゲーム開始");
				f->set_scale(glm::vec2(1, -1));
				startPlate->add_child(f);
			}
			mRoot->get_child_by_tag(mSelectTag)->run_action(
				repeat_forever::create(
					sequence::create(
						ease<ci::EaseInOutCirc>::create(scale_by::create(0.26F, ci::vec2(0.2F))),
						ease<ci::EaseInOutCirc>::create(scale_by::create(0.26F, ci::vec2(-0.2F)))
					)
				)
			);
		}

		void cMatching::draw()
		{

		}

		void cMatching::draw2D()
		{
			ci::gl::clear(ci::ColorA(0, 0, 0, 1));
			if (mBeginAnimation != true)
			{
				mRoot->entry_render(cinder::mat4());
				for (auto& m : stars)
					m.draw();
				for (auto& m : drillUI1Ps)
					m.draw();
				for (auto& m : drillUI2Ps)
					m.draw();
			}
			drawInRoom2D();
		}

		void cMatching::drawInRoom2D()
		{
			if (mPhaseState != PhaseState::IN_ROOM)return;
			if (mBeginAnimation != true)
				mMemberRoot->entry_render(cinder::mat4());
			if (mBeginAnimation == true)
			{
				ci::gl::ScopedTextureBind p(mTrimeshAnimationFbo->getColorTexture());
				ci::gl::ScopedGlslProg p2(ci::gl::getStockShader(ci::gl::ShaderDef().texture()));
				mTrimeshAnimation.draw();
			}
				
		}

		void cMatching::resize()
		{

		}
	}
}
