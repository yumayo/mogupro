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

using namespace Network;
using namespace Network::Packet::Event;
using namespace Network::Packet::Request;
using namespace Network::Packet::Response;
using namespace Node::Action;
void drawRect(const ci::vec2& pos, const ci::vec2& size, const ci::ColorA& color)
{
	ci::gl::color(color);
	ci::gl::drawSolidRect(ci::Rectf(ci::vec2(pos.x - size.x / 2.0f, pos.y - size.y / 2.0f),
		ci::vec2(pos.x + size.x / 2.0f, pos.y + size.y / 2.0f)));
}

bool BoxToMouse(const ci::vec2& posA, const ci::vec2& sizeA, const ci::vec2& mousePos)
{
	if (posA.x - sizeA.x / 2 > mousePos.x)return false;
	if (posA.x + sizeA.x / 2 < mousePos.x)return false;
	if (posA.y - sizeA.y / 2 > mousePos.y)return false;
	if (posA.y + sizeA.y / 2 < mousePos.y)return false;

	return true;
}

namespace Scene
{
	namespace Member
	{
		void cMatching::setup()
		{
			Network::cUDPClientManager::getInstance()->open();
			cUDPClientManager::getInstance()->connect("10.25.31.8");
			mClassState = ClassState::NOT;
			mWaitClassState = ClassState::NOT;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mCanSend = true;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mSelectTag = 0;
			mPrevSelectTag = 0;
			mAddMember = false;
			mTeamNum = -1;
			mBeginAnimation = false;
			registerFunc();
			mTrimeshAnimationFbo = ci::gl::Fbo::create(ci::app::getWindowWidth(),
				ci::app::getWindowHeight(), true);
			Resource::TextureManager::getInstance()->set("nightSky.png", "nightSky.png");
			sceneChange = false;
		}

		void cMatching::registerFunc()
		{
			mRoot = Node::node::create();
			mRoot->set_schedule_update();
			auto backView = Node::Renderer::sprite::create("nightSky.png");
			backView->set_position(ci::vec2(0, 0));
			backView->set_scale(ci::vec2(2.0f, -1.5f));
			mRoot->add_child(backView);
			auto makeRoom = Node::Renderer::rect::create(ci::vec2(300, 300));
			makeRoom->set_position(ci::vec2(-250, 0));
			makeRoom->set_color(ci::ColorA(1, 0, 0));
			makeRoom->set_tag(0);
			makeRoom->set_schedule_update();
			mRoot->add_child(makeRoom);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
				f->set_text(u8"部屋を作る");
				f->set_scale(glm::vec2(1, -1));
				makeRoom->add_child(f);
			}
			outRoomFunc.emplace_back(
				[this] {
				mCanSend = false;
				cUDPClientManager::getInstance()->send(new cReqMakeRoom(100));
				mWaitClassState = ClassState::MASTER;
			});

			auto inRoom = Node::Renderer::rect::create(ci::vec2(300, 300));
			inRoom->set_position(ci::vec2(250, 0));
			inRoom->set_color(ci::ColorA(0, 1, 0));
			inRoom->set_tag(1);
			inRoom->set_schedule_update();
			mRoot->add_child(inRoom);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
				f->set_text(u8"部屋に入る");
				f->set_scale(glm::vec2(1, -1));
				inRoom->add_child(f);
			}
			outRoomFunc.emplace_back(
				[this] {
				cUDPClientManager::getInstance()->send(new cReqInRoom(100));
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
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
				f->set_color(ci::ColorA(1, 0, 0));
				f->set_position(ci::vec2(-350, 250));
				f->set_text(u8"東軍");
				f->set_scale(glm::vec2(1, -1));
				mMemberRoot->add_child(f);
			}
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
				f->set_position(ci::vec2(350, 250));
				f->set_text(u8"西軍");
				f->set_scale(glm::vec2(1, -1));
				mMemberRoot->add_child(f);
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
			for (auto& m : drillUI1Ps)
				m.update(deltaTime);
			for (auto& m : drillUI2Ps)
				m.update(deltaTime);
			if (mBeginAnimation == true)
				mTrimeshAnimation.update();
			Network::cUDPClientManager::getInstance()->update(deltaTime);
			if (cUDPClientManager::getInstance()->isConnected() == false)return;
			makeRoom();
			inRoom();
			updateBoxFunc();
			if (sceneChange == true)
			{
				shutDown();
				cSceneManager::getInstance()->change<Scene::Member::cGameMain>();
				cSceneManager::getInstance()->now().setup();
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
				while (auto resCheckBeginGame = cResponseManager::getInstance()->getResCheckBeginGame())
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
				while (auto resMakeRoom = cResponseManager::getInstance()->getResMakeRoom())
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
				while (auto resInRoom = cResponseManager::getInstance()->getResInRoom())
				{
					if (resInRoom->mFlag = false)
					{
						mWaitClassState = ClassState::NOT;
						mCanSend = true;
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
				while (auto resMakeRoom = cResponseManager::getInstance()->getResMakeRoom())
				{
					if (resMakeRoom->mFlag == false)
					{
						mWaitClassState = ClassState::NOT;
						mCanSend = true;
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
			while (auto resWantTeamIn = cResponseManager::getInstance()->getResWantTeamIn())
			{
				//!@ TODO : Add Performance (Team%Dに入れました or Team%Dに入れませんでした)
				if (resWantTeamIn->mFlag == 1)
					mTeamNum = resWantTeamIn->mTeamNum;
				mCanSend = true;
				if (resWantTeamIn->mTeamNum == 0)
					drillUI1Ps.push_back(DrillUI(ci::vec2(-1000, 200 - 200 * drillUI1Ps.size()),
						ci::vec2(-200, 200 - 200 * drillUI1Ps.size()), "Mikuyama"));
				else
					drillUI2Ps.push_back(DrillUI(ci::vec2(1000, 200 - 200 * drillUI1Ps.size()),
						ci::vec2(200, 200 - 200 * drillUI1Ps.size()), "Mikuyama"));
			}
			//TODO : 参加した場合とTeamが変更された場合は分けるべき
			while (auto eveTeamMember = cEventManager::getInstance()->getEveTeamMember())
			{
				cMatchingMemberManager::getInstance()->addPlayerDatas(
					eveTeamMember->mNameStr, eveTeamMember->mTeamNum, eveTeamMember->mPlayerID, cNetworkHandle("", 0));
				mAddMember = true;
				if (eveTeamMember->mTeamNum == 0)
					drillUI1Ps.push_back(DrillUI(ci::vec2(-1000, 200 - 200 * drillUI1Ps.size()),
						ci::vec2(-200, 200 - 200 * drillUI1Ps.size()), eveTeamMember->mNameStr));
				else
					drillUI2Ps.push_back(DrillUI(ci::vec2(1000, 200 - 200 * drillUI1Ps.size()),
						ci::vec2(200, 200 - 200 * drillUI1Ps.size()), eveTeamMember->mNameStr));

			}
		}

		void cMatching::addInRoomUI()
		{
			mRoot->remove_all_children();
			auto backView = Node::Renderer::sprite::create("nightSky.png");
			backView->set_tag(1);
			backView->set_position(ci::vec2(0, 0));
			backView->set_scale(ci::vec2(1.0f, -1.0f));
			mRoot->add_child(backView);
			mSelectTag = 0;

			if (mClassState != ClassState::MASTER)
				return;

			auto start = Node::Renderer::rect::create(ci::vec2(150, 150));
			start->set_position(ci::vec2(500, -200));
			start->set_color(ci::ColorA(0, 1, 0));
			start->set_tag(0);
			start->set_schedule_update();
			mRoot->add_child(start);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
				f->set_text(u8"ゲーム開始");
				f->set_scale(glm::vec2(1, -1));
				start->add_child(f);
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
