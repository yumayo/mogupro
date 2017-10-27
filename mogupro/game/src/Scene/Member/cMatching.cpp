#include <Scene/Member/cMatching.h>
#include "cinder/gl/gl.h"
#include <Utility/cInput.h>
#include <string>
#include <Network.hpp>
#include <CameraManager/cCameraManager.h>
#include <Scene/Member/cGameMain.h>
#include <Scene/cSceneManager.h>
#include <Network/cMatchingMemberManager.h>
using namespace Network;
using namespace Network::Packet::Event;
using namespace Network::Packet::Request;
using namespace Network::Packet::Response;
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
			cUDPClientManager::getInstance()->connect("10.25.36.137");
			mClassState = ClassState::NOT;
			mWaitClassState = ClassState::NOT;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mCanSend = true;
			ci::vec3 pos = ci::vec3(0);
			CAMERA->followingCamera(&pos, 30);
			CAMERA->setup();
			ENV->padSetup();
			using namespace Node::Action;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			font = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 40);
			font->set_text(u8"");
			font->set_scale(glm::vec2(1, -1));
			smallFont = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 20);
			smallFont->set_text(u8"");
			smallFont->set_scale(glm::vec2(1, -1));
			n = Node::node::create();
			n->add_child(font);
			n->add_child(smallFont);
		}

		void cMatching::shutDown()
		{

		}

		void cMatching::update(float deltaTime)
		{

			Network::cUDPClientManager::getInstance()->update(deltaTime);
			if (cUDPClientManager::getInstance()->isConnected() == false)return;
			makeRoom();
			inRoom();
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

				if (ENV->pushKey(ci::app::KeyEvent::KEY_z))
				{
					mCanSend = false;
					cUDPClientManager::getInstance()->send(new cReqMakeRoom(100));
					mWaitClassState = ClassState::MASTER;
				}

				else if (ENV->pushKey(ci::app::KeyEvent::KEY_x))
				{
                    cUDPClientManager::getInstance()->send(new cReqInRoom(100));
					mCanSend = false;
					mWaitClassState = ClassState::CLIENT;
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
						cResponseManager::getInstance()->mResInRoom.pop();
						continue;
					}

					mWaitClassState = ClassState::NOT;
					mCanSend = true;
					mClassState = ClassState::CLIENT;
					mPhaseState = PhaseState::IN_ROOM;
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
				}
			}
		}
		void cMatching::inRoom()
		{
			if (mPhaseState != PhaseState::IN_ROOM)return;

			if (mClassState == ClassState::CLIENT
				|| mClassState == ClassState::MASTER)
			{

				if (mCanSend == true)
				{
					//1P・2Pのどっちに入るのかの選択
					if (ENV->pushKey(ci::app::KeyEvent::KEY_z))
					{
						cUDPClientManager::getInstance()->send(new cReqWantTeamIn(0));
						mCanSend = false;
					}

					else if (ENV->pushKey(ci::app::KeyEvent::KEY_x))
					{
						cUDPClientManager::getInstance()->send(new cReqWantTeamIn(1));
						mCanSend = false;
					}
				}
				while (auto resWantTeamIn = cResponseManager::getInstance()->getResWantTeamIn())
				{
					if (resWantTeamIn->mFlag == true)
					{
						mCanSend = true;
					}
					else
					{
						mCanSend = true;
					}
				}
				
				while (auto resCheckBeginGame = cResponseManager::getInstance()->getResCheckBeginGame())
				{	
                    cMatchingMemberManager::getInstance( )->mPlayerID = resCheckBeginGame->mPlayerID;
                    shutDown();
					cSceneManager::getInstance()->change<Scene::Member::cGameMain>();
					cSceneManager::getInstance()->now().setup();
					continue;
				}

				while (auto eveTeamMember = cEventManager::getInstance()->getEveTeamMember())
				{
					cMatchingMemberManager::getInstance()->addPlayerDatas(
						eveTeamMember->mNameStr,eveTeamMember->mTeamNum,eveTeamMember->mPlayerID);
				}
			}

			if (mClassState == ClassState::MASTER)
			{
				if (ENV->pushKey(ci::app::KeyEvent::KEY_c))
				{
					cUDPClientManager::getInstance()->send(new cReqCheckBeginGame());
					mCanSend = false;
				}
			}
		}

		void cMatching::draw()
		{

		}
		void cMatching::draw2D()
		{
			switch (mPhaseState)
			{
			case PhaseState::NOT_IN_ROOM:
				drawRect(ci::vec2(-300, 0), ci::vec2(300, 300), ci::ColorA(0, 1, 0, 1));
				font->set_text(u8"ルームを作る");
				font->set_position_3d(glm::vec3(-300, 0, 0));
				ci::gl::pushModelView();
				n->entry_render(ci::mat4());
				ci::gl::popModelView();
				font->set_text(u8"Z key");
				font->set_position_3d(glm::vec3(-300, -50, 0));
				ci::gl::pushModelView();
				n->entry_render(ci::mat4());
				ci::gl::popModelView();
				drawRect(ci::vec2(300, 0), ci::vec2(300, 300), ci::ColorA(0, 0, 1, 1));
				font->set_text(u8"ルームに入る");
				font->set_position_3d(glm::vec3(300, 0, 0));
				ci::gl::pushModelView();
				n->entry_render(ci::mat4());
				ci::gl::popModelView();
				font->set_text(u8"X key");
				font->set_position_3d(glm::vec3(300, -50, 0));
				ci::gl::pushModelView();
				n->entry_render(ci::mat4());
				ci::gl::popModelView();
				break;
			case PhaseState::IN_ROOM:
			
				break;
			}
		}
		void cMatching::resize()
		{

		}
	}
}
