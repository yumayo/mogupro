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
			cUDPClientManager::getInstance()->connect("10.25.34.217");
			mClassState = ClassState::NOT;
			mWaitClassState = ClassState::NOT;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mCanSend = true;
			ENV->padSetup();
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mSelectTag = 0;
			mPrevSelectTag = 0;
			mAddMember = false;
			mTeamNum = -1;

			registerFunc();
		}

		void cMatching::registerFunc()
		{
			mRoot = Node::node::create();
			mRoot->set_schedule_update();

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
				cUDPClientManager::getInstance()->send(new cReqWantTeamIn(0));
				mCanSend = false;
			});

			inRoomFunc.emplace_back(
				[this] {
				cUDPClientManager::getInstance()->send(new cReqWantTeamIn(1));
				mCanSend = false;
			});

			inRoomFunc.emplace_back(
				[this] {
				if (mClassState == ClassState::MASTER)
				{
					cUDPClientManager::getInstance()->send(new cReqCheckBeginGame());
					mCanSend = false;
				}
			});

			mMemberRoot = Node::node::create();
			mMemberRoot->set_schedule_update();
		}


		void cMatching::shutDown()
		{

		}

		void cMatching::update(float deltaTime)
		{
			mPrevSelectTag = mSelectTag;
			mRoot->entry_update(deltaTime);
			Network::cUDPClientManager::getInstance()->update(deltaTime);
			if (cUDPClientManager::getInstance()->isConnected() == false)return;
			makeRoom();
			inRoom();
			updateBoxFunc();
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
					shutDown();
					cSceneManager::getInstance()->change<Scene::Member::cGameMain>();
					cSceneManager::getInstance()->now().setup();
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
					mRoot->remove_all_children();
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
					mRoot->remove_all_children();
					addInRoomUI();
				}
			}
		}

		void cMatching::inRoom()
		{
			if (mPhaseState != PhaseState::IN_ROOM)return;
			mAddMember = false;
			if (mClassState == ClassState::NOT)return;


			if (ENV->pushKey(ci::app::KeyEvent::KEY_RIGHT))
			{
				if (mClassState == ClassState::MASTER)
					mSelectTag = std::min(mSelectTag + 1, 2);
				else
					mSelectTag = std::min(mSelectTag + 1, 1);
			}

			if (ENV->pushKey(ci::app::KeyEvent::KEY_LEFT))
				mSelectTag = std::max(mSelectTag - 1, 0);

			if (ENV->pushKey(ci::app::KeyEvent::KEY_RETURN) && mCanSend)
				inRoomFunc[mSelectTag]();

			//Teamに入れたかどうか
			while (auto resWantTeamIn = cResponseManager::getInstance()->getResWantTeamIn())
			{
				//!@ TODO : Add Performance (Team%Dに入れました or Team%Dに入れませんでした)
				if (resWantTeamIn->mFlag == 1)
					mTeamNum = resWantTeamIn->mTeamNum;
			
				mCanSend = true;
			}
			//TODO : 参加した場合とTeamが変更された場合は分けるべき
			while (auto eveTeamMember = cEventManager::getInstance()->getEveTeamMember())
			{
				cMatchingMemberManager::getInstance()->addPlayerDatas(
					eveTeamMember->mNameStr, eveTeamMember->mTeamNum, eveTeamMember->mPlayerID, cNetworkHandle("", 0));
				mAddMember = true;
			}
		}

		void cMatching::addInRoomUI()
		{
			auto san = Node::Renderer::rect::create(ci::vec2(150, 150));
			san->set_position(ci::vec2(100, -200));
			san->set_color(ci::ColorA(1, 0, 0));
			san->set_tag(0);
			san->set_schedule_update();
			mRoot->add_child(san);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
				f->set_text(u8"赤組に入る");
				f->set_scale(glm::vec2(1, -1));
				san->add_child(f);
			}

			auto moon = Node::Renderer::rect::create(ci::vec2(150, 150));
			moon->set_position(ci::vec2(300, -200));
			moon->set_color(ci::ColorA(1, 1, 1));
			moon->set_tag(1);
			moon->set_schedule_update();
			mRoot->add_child(moon);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
				f->set_color(ci::ColorA(0, 0, 0));
				f->set_text(u8"白組に入る");
				f->set_scale(glm::vec2(1, -1));
				moon->add_child(f);
			}

			mSelectTag = 0;
			mRoot->get_child_by_tag(mSelectTag)->run_action(
				repeat_forever::create(
					sequence::create(
						ease<ci::EaseInOutCirc>::create(scale_by::create(0.26F, ci::vec2(0.2F))),
						ease<ci::EaseInOutCirc>::create(scale_by::create(0.26F, ci::vec2(-0.2F)))
					)
				)
			);

			if (mClassState != ClassState::MASTER)
				return;

			auto start = Node::Renderer::rect::create(ci::vec2(150, 150));
			start->set_position(ci::vec2(500, -200));
			start->set_color(ci::ColorA(0, 1, 0));
			start->set_tag(2);
			start->set_schedule_update();
			mRoot->add_child(start);
			{
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
				f->set_text(u8"ゲーム開始");
				f->set_scale(glm::vec2(1, -1));
				start->add_child(f);
			}
		}

		void cMatching::draw()
		{

		}

		void cMatching::draw2D()
		{
			mRoot->entry_render(cinder::mat4());
			drawInRoom2D();
			mMemberRoot->entry_render(cinder::mat4());
		}

		void cMatching::drawInRoom2D()
		{
			if (mPhaseState != PhaseState::IN_ROOM)return;
			int noTeamCount = 0;
			int team1Count = 0;
			int team2Count = 0;
			for each (auto m in cMatchingMemberManager::getInstance()->mPlayerDatas)
			{
				auto nameTag = Node::Renderer::rect::create(ci::vec2(300, 50));
				auto f = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 32);
				if (m.teamNum == 0)
				{
					nameTag->set_color(ci::ColorA(1, 0, 0));
					nameTag->set_position(ci::vec2(-350, 200 - 60 * team1Count));
					team1Count++;
				}

				else if (m.teamNum == 1)
				{
					nameTag->set_color(ci::ColorA(1, 1, 1));
					nameTag->set_position(ci::vec2(350, 200 - 60 * team2Count));
					team2Count++;
					f->set_color(ci::ColorA(0, 0, 0));
				}

				else
				{
					nameTag->set_color(ci::ColorA(0, 1, 0));
					nameTag->set_position(ci::vec2(0, 200 - 60 * noTeamCount));
					noTeamCount++;
					f->set_color(ci::ColorA(0, 0, 0));
				}

				mMemberRoot->add_child(nameTag);
				f->set_text(u8"" + m.nameStr);
				f->set_scale(glm::vec2(1, -1));
				nameTag->add_child(f);
			}

		}

		void cMatching::resize()
		{

		}
	}
}
