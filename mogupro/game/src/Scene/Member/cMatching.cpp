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
#include <Scene/Member/cTitle.h>
#include <Resource/cSoundManager.h>

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
	mRoot->run_action(sequence::create(ease<ci::EaseOutCirc>::create(
		move_to::create(3.0F, ci::vec3(moveVec.x, moveVec.y, 0))),
		call_func::create([this] {
	
		})));

	hardptr<Node::Renderer::sprite> plate;

	plate = Node::Renderer::sprite::create(Resource::IMAGE["matching/drillUI.png"]);

	plate->set_position(ci::vec2(0, 0));
	plate->set_scale(glm::vec2(1.6f, 1.0f));

	auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
	f->set_color(ci::Color(0,0,0));
	f->set_text(u8"" + name);
	f->set_position(ci::vec2(0, 0));
	f->set_schedule_update();
	f->set_scale(glm::vec2(1, -1));
	auto fire = Node::Renderer::sprite::create(Resource::IMAGE["matching/fire1.png"]);
	fire->set_tag(10);
	if (pos.x < 0)
	{
		firePosition = ci::vec2(-189,-4);
	}
	else
	{
		firePosition = ci::vec2(-189, -4);
		mRoot->set_rotation(M_PI);
		f->set_rotation(M_PI);
	}
	mRoot->add_child(plate);
	mRoot->add_child(f);
	fire->set_position(firePosition);
	mRoot->add_child(fire);
	animationTime = 0;
	type = AnimationType::BEGIN;
	time = 3.0f;
}

void DrillUI::update(float deltaTime)
{

	mRoot->entry_update(deltaTime);
	if (type == AnimationType::NONE)return;
	animationTime++;
	time -= deltaTime;
	if (type == AnimationType::BEGIN)
	{
		int f = animationTime / 10;
		int flame = ((int)f) % 4 + 1;
		if (flame == 4)
			flame = 1;
		mRoot->remove_child_by_tag(10);
		if (time < 0)
		{
			type = AnimationType::NONE;
			animationTime = 0;
			return;
		}
		auto fire = Node::Renderer::sprite::create(Resource::IMAGE["matching/fire" + std::to_string(flame) + ".png"]);
		fire->set_tag(10);
		fire->set_position(firePosition);
		mRoot->add_child(fire);
	}
	else if (type == AnimationType::END)
	{
		if (time > 3.5f)return;
		int f = animationTime / 10;
		int flame = ((int)f) % 3;
		//05, 06, 03
		if (flame == 0)
			flame = 5;
		else if (flame == 1)
			flame = 6;
		else
			flame = 3;
		mRoot->remove_child_by_tag(10);
		auto fire = Node::Renderer::sprite::create(Resource::IMAGE["matching/fire" + std::to_string(flame) + ".png"]);
		fire->set_tag(10);
		fire->set_position(firePosition);
		mRoot->add_child(fire);
	}
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
			mClassState = ClassState::NOT;
			mWaitClassState = ClassState::NOT;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mCanSend = true;
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
			wantWatching = false;
			canSendInRoom = false;
			canStartUpdateServer = false;
		}

		void cMatching::registerFunc()
		{
			mBackRoot = Node::node::create();
			mBackRoot->set_schedule_update();
			auto backView = Node::Renderer::sprite::create(Resource::IMAGE["matching/nightSky.png"]);
			backView->set_position(ci::vec2(0, 0));
			backView->set_scale(ci::vec2(2.0f, -1.5f));
			mBackRoot->add_child(backView);
			
			mRoot = Node::node::create();
			mRoot->set_schedule_update();

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
				Network::cUDPClientManager::getInstance()->open();
				cUDPClientManager::getInstance()->connect(Resource::JSON["server.json"]["ip"].asString());
				canSendInRoom = true;
			});

			//タイトルに戻るボタンUI
			auto lookPlate = Node::Renderer::sprite::create(Resource::IMAGE["matching/redUI.png"]);
			lookPlate->set_schedule_update();
			lookPlate->set_tag(1);
			lookPlate->set_position(ci::vec2(250, 0));
			mRoot->add_child(lookPlate);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 30);
				f->set_text(u8"タイトルに戻る");
				f->set_scale(glm::vec2(1, -1));
				lookPlate->add_child(f);
			}

			outRoomFunc.emplace_back(
				[this] 
			{
				sceneChange = true;
				sceneType = SceneType::TITLE;
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
		}
		void cMatching::setAnimation()
		{
			auto m = Node::node::create();
			m->set_schedule_update();
			m->run_action(sequence::create(
				call_func::create([this] {
				float randPos[3][3];
				float randTime[3][3];

				for (int i = 0; i < 3; i++)
				{
					for (int k = 0; k < 3; k++)
					{
						randPos[i][k] = ci::randFloat(-45.0f,45.0f);
						randTime[i][k] = ci::randFloat(0.0f, 3.0f);
					}
				}

				int c = 0;
				for (auto& m : drillUI1Ps)
				{
					ci::vec2 pos = m.mRoot->get_position();
					m.mRoot->run_action(sequence::create(move_to::create(3.5F, ci::vec3(-2000, pos.y, 0)),
						ease<ci::EaseOutCirc>::create(move_to::create(4.0F, ci::vec3(-250, pos.y, 0))),
						ease<ci::EaseOutCirc>::create(move_to::create(randTime[c][0], ci::vec3(randPos[c][0], pos.y, 0))),
						ease<ci::EaseOutCirc>::create(move_to::create(randTime[c][1], ci::vec3(randPos[c][1], pos.y, 0))),
						ease<ci::EaseOutCirc>::create(move_to::create(randTime[c][2], ci::vec3(randPos[c][2], pos.y, 0)))));
					m.time = 7.5f;
					m.type = DrillUI::AnimationType::END;
					c++;
				}
				c = 0;
				for (auto& m : drillUI2Ps)
				{
					ci::vec2 pos = m.mRoot->get_position();
					m.mRoot->run_action(sequence::create(move_to::create(3.5F, ci::vec3(2000, pos.y, 0)),
						ease<ci::EaseOutCirc>::create(move_to::create(4.0F, ci::vec3(250, pos.y, 0))),
						ease<ci::EaseOutCirc>::create(move_to::create(randTime[c][0], ci::vec3(-randPos[c][0], pos.y, 0))),
						ease<ci::EaseOutCirc>::create(move_to::create(randTime[c][1], ci::vec3(-randPos[c][1], pos.y, 0))),
						ease<ci::EaseOutCirc>::create(move_to::create(randTime[c][2], ci::vec3(-randPos[c][2], pos.y, 0)))));
					m.time = 7.5f;
					m.type = DrillUI::AnimationType::END;
					c++;
				}
			}),
				delay::create(7.5f),
				//ヒビ入れる１
				call_func::create([this] {
				Resource::SE["Matching/h.wav"].play();
				auto h = Node::Renderer::sprite::create(Resource::IMAGE["matching/h1.png"]);
				h->set_schedule_update();
				h->set_position(ci::vec2(-200, 400));
				mMemberRoot->add_child(h);
			}),
				delay::create(1.2f),
				//ヒビ入れる２
				call_func::create([this] {
				Resource::SE["Matching/h.wav"].play();
				auto h = Node::Renderer::sprite::create(Resource::IMAGE["matching/h2.png"]);
				h->set_schedule_update();
				h->set_position(ci::vec2(250, -450));
				mMemberRoot->add_child(h);
			}),
				delay::create(0.8f),
				//ヒビ入れる3
				call_func::create([this] {
				Resource::SE["Matching/h.wav"].play();
				auto h = Node::Renderer::sprite::create(Resource::IMAGE["matching/h1.png"]);
				h->set_schedule_update();
				h->set_position(ci::vec2(0, 80));
				mMemberRoot->add_child(h);
			}),
				delay::create(0.5f),
				call_func::create([this] {
				//ヒビ入れる4 	
				Resource::SE["Matching/p.wav"].play();
			}),
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
				mMemberRoot->entry_render(cinder::mat4());
				mTrimeshAnimation.make(ci::vec2(1600, 900), ci::vec2(20, 15));
				mBeginAnimation = true;
			}),
				delay::create(4.0f), 
				call_func::create([this]
			{
				sceneChange = true;
				sceneType = SceneType::GAME_MAIN;
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
			if (canSendInRoom == true)
			{
				canSendInRoom = false;
				cUDPClientManager::getInstance()->send(new cReqMakeRoom(100));
				mWaitClassState = ClassState::MASTER;
				canStartUpdateServer = true;
			}

			if (canStartUpdateServer == true)
			{
				Network::cUDPClientManager::getInstance()->update(deltaTime);
				if (cUDPClientManager::getInstance()->isConnected() == false)return;
			}

			if (wantWatching == true)
			{
				Network::cUDPClientManager::getInstance()->update(deltaTime);
				if (cUDPClientManager::getInstance()->isConnected() == true)
				{
					cUDPClientManager::getInstance()->send(new cReqInRoomWatching(100));
					mCanSend = false;
					mWaitClassState = ClassState::CLIENT;
					mSelectTag = 0;
					wantWatching = false;
				}
			}

			makeRoom();
			inRoom();
			updateBoxFunc();
			if (sceneChange == true)
			{
				if(sceneType == SceneType::TITLE)
					cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
				else if(sceneType == SceneType::GAME_MAIN)
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

				if (ENV->pushKey(ci::app::KeyEvent::KEY_RIGHT) || ENV->getPadAxisPushMinus(0))
				{
					mSelectTag = std::min(mSelectTag + 1, 1);
				}
				 
				if (ENV->pushKey(ci::app::KeyEvent::KEY_LEFT) || ENV->getPadAxisPushPlus(0))
				{
					mSelectTag = std::max(mSelectTag - 1, 0);
				}

				if (ENV->pushKey(ci::app::KeyEvent::KEY_RETURN) || ENV->isPadPush(0) && mCanSend)
				{
					Resource::SE["Matching/click.wav"].play();
					outRoomFunc[mSelectTag]();
				}

				if (ENV->pressKey(ci::app::KeyEvent::KEY_a) && ENV->pressKey(ci::app::KeyEvent::KEY_s) && mCanSend)
				{
					Network::cUDPClientManager::getInstance()->open();
					cUDPClientManager::getInstance()->connect(Resource::JSON["server.json"]["ip"].asString());
					//!@LookMe : クソ怪しい処理ではある。
					/*cUDPClientManager::getInstance()->send(new cReqInRoomWatching(100));
					mCanSend = false;
					mWaitClassState = ClassState::CLIENT;
					mSelectTag = 0;
					wantWatching = true;*/
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

			if (ENV->pushKey(ci::app::KeyEvent::KEY_RETURN) || ENV->isPadPush(0) && mCanSend)
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
						drillUI1Ps.push_back(DrillUI(ci::vec2(-1000, 140 - 130 * teamCount[0]),
							ci::vec2(-390, 140 - 130 * teamCount[0]), "You"));

					else if (resWantTeamIn->mTeamNum == 1)
						drillUI2Ps.push_back(DrillUI(ci::vec2(1000, 140 - 160 * teamCount[1]),
							ci::vec2(390, 140 - 160 * teamCount[1]), "You"));
					teamCount[mTeamNum]++;
			}
			//TODO : 参加した場合とTeamが変更された場合は分けるべき
			int count = 0;
			while (auto eveTeamMember = m->EveTeamMember.get())
			{
				cMatchingMemberManager::getInstance()->addPlayerDatas(
					eveTeamMember->mNameStr, eveTeamMember->mTeamNum, eveTeamMember->mPlayerID, cNetworkHandle("", 0));
				
				if (eveTeamMember->mPlayerID != 3 && eveTeamMember->mPlayerID != 7)
				{
					if (eveTeamMember->mTeamNum == 0)
						drillUI1Ps.push_back(DrillUI(ci::vec2(-1000, 140 - 130 * teamCount[0]),
							ci::vec2(-390, 140 - 160 * teamCount[0]), u8"もぐら" + std::to_string(eveTeamMember->mPlayerID)));
					else if (eveTeamMember->mTeamNum == 1)
						drillUI2Ps.push_back(DrillUI(ci::vec2(1000, 140 - 130 * teamCount[1]),
							ci::vec2(390, 140 - 160 * teamCount[1]), u8"もぐら" + std::to_string(eveTeamMember->mPlayerID)));
					teamCount[eveTeamMember->mTeamNum]++;
					++count;
				}	
			}
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
			{
				auto lookPlate = Node::Renderer::sprite::create(Resource::IMAGE["matching/eastPlate.png"]);
				lookPlate->set_position(ci::vec2(-350, 0));
				lookPlate->set_scale(glm::vec2(1, -1));
				mRoot->add_child(lookPlate);
			}
			{
				auto plate = Node::Renderer::sprite::create(Resource::IMAGE["matching/westPlate.png"]);
				plate->set_position(ci::vec2(350,0));
				plate->set_scale(glm::vec2(1, -1));
				mRoot->add_child(plate);
			}
			if (mClassState != ClassState::MASTER)
				return;

			auto startPlate = Node::Renderer::sprite::create(Resource::IMAGE["matching/greenUI2.png"]);
			startPlate->set_schedule_update();
			//startPlate->set_scale(glm::vec2(1, -1));
			startPlate->set_tag(0);
			startPlate->set_position(ci::vec2(500, -250));
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
				mBackRoot->entry_render(cinder::mat4());
				for (auto& m : stars)
					m.draw();
				mRoot->entry_render(cinder::mat4());
				
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
