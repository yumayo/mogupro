#include <Scene/Member/cMatching.h>
#include "cinder/gl/gl.h"
#include <Utility/cInput.h>
#include <string>
#include <Network.hpp>
#include <CameraManager/cCameraManager.h>
#include <Scene/Member/cGameMain.h>
#include <Scene/cSceneManager.h>
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
			mClassState = ClassState::NOT;
			mWaitClassState = ClassState::NOT;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mCanSend = true;
			Network::cUDPManager::getInstance()->open();
			ci::vec3 pos = ci::vec3(0);
			CAMERA->followingCamera(&pos, 30);
			CAMERA->setup();
			ENV->padSetup();
			//mFont = ci::Font("timesi.ttf",20);
		}

		void cMatching::shutDown()
		{

		}

		void cMatching::update(float deltaTime)
		{
			Network::cUDPManager::getInstance()->update();
			makeRoom();
			inRoom();
		}

		void cMatching::makeRoom()
		{
			if (mPhaseState != PhaseState::NOT_IN_ROOM)return;

			if (mWaitClassState == ClassState::NOT)
			{
				while (!cResponseManager::getInstance()->mResMakeRoom.empty())
				{
					auto resMakeRoom = cResponseManager::getInstance()->mResMakeRoom.top();
					cResponseManager::getInstance()->mResMakeRoom.pop();
					continue;
				}

				if (ENV->pushKey(ci::app::KeyEvent::KEY_z))
				{
					mCanSend = false;
					cUDPManager::getInstance()->send(cNetworkHandle("10.25.36.137", 25565), new cReqMakeRoom(100));
					mWaitClassState = ClassState::MASTER;
				}

				else if (ENV->pushKey(ci::app::KeyEvent::KEY_x))
				{
					cUDPManager::getInstance()->send(cNetworkHandle("10.25.36.137", 25565), new cReqInRoom(100));
					mCanSend = false;
					mWaitClassState = ClassState::CLIENT;
				}
			}
			else if (mWaitClassState == ClassState::CLIENT)
			{
				while (!cResponseManager::getInstance()->mResInRoom.empty())
				{
					auto resMakeRoom = cResponseManager::getInstance()->mResInRoom.top();
					if (resMakeRoom.mFlag = false)
					{
						mWaitClassState = ClassState::NOT;
						mCanSend = true;
						cResponseManager::getInstance()->mResInRoom.pop();
						continue;
					}

					mWaitClassState = ClassState::NOT;
					mCanSend = true;
					mClassState = ClassState::CLIENT;
					cResponseManager::getInstance()->mResInRoom.pop();
					continue;
				}
			}

			else if (mWaitClassState == ClassState::MASTER)
			{
				while (!cResponseManager::getInstance()->mResMakeRoom.empty())
				{
					auto resMakeRoom = cResponseManager::getInstance()->mResMakeRoom.top();
					if (resMakeRoom.mFlag = false)
					{
						mWaitClassState = ClassState::NOT;
						mCanSend = true;
						cResponseManager::getInstance()->mResMakeRoom.pop();
						continue;
					}

					mWaitClassState = ClassState::NOT;
					mCanSend = true;
					mClassState = ClassState::MASTER;
					cResponseManager::getInstance()->mResMakeRoom.pop();
					continue;
				}
			}
		}
		void cMatching::inRoom()
		{

			if (mClassState == ClassState::CLIENT
				|| mClassState == ClassState::MASTER)
			{

				if (mCanSend == true)
				{
					//1PE2P‚Ì‚Ç‚Á‚¿‚É“ü‚é‚Ì‚©‚Ì‘I‘ð
					if (ENV->pushKey(ci::app::KeyEvent::KEY_1))
					{
						cUDPManager::getInstance()->send(cNetworkHandle("10.25.36.137", 25565),
							new cReqWantTeamIn(0));
						mCanSend = false;
					}

					else if (ENV->pushKey(ci::app::KeyEvent::KEY_2))
					{
						cUDPManager::getInstance()->send(cNetworkHandle("10.25.36.137", 25565),
							new cReqWantTeamIn(1));
						mCanSend = false;
					}
				}
				while (!cResponseManager::getInstance()->mResWantTeamIn.empty())
				{
					auto resWantTeamIn = cResponseManager::getInstance()->mResWantTeamIn.top();
					if (resWantTeamIn.mFlag == true)
					{
						mCanSend = true;
					}
					else
					{
						mCanSend = true;
					}
				}
				while (!cResponseManager::getInstance()->mResWantTeamIn.empty())
				{
					auto resWantTeamIn = cResponseManager::getInstance()->mResWantTeamIn.top();
					if (resWantTeamIn.mFlag == true)
					{
						mCanSend = true;
					}
					else
					{
						mCanSend = true;
					}
				}

				while (!cResponseManager::getInstance()->mResCheckBeginGame.empty())
				{
					auto resWantTeamIn = cResponseManager::getInstance()->mResCheckBeginGame.top();
					cRequestManager::getInstance()->mReqWantTeamIn.pop();
					shutDown();
					cSceneManager::getInstance()->change<Scene::Member::cGameMain>();
					cSceneManager::getInstance()->now().setup();
					continue;
				}
			}

			if (mClassState == ClassState::MASTER)
			{
				if (ENV->pushKey(ci::app::KeyEvent::KEY_3))
				{
					cUDPManager::getInstance()->send(cNetworkHandle("10.25.36.137", 25565), new cReqCheckBeginGame());
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
				drawRect(ci::vec2(-300, -100), ci::vec2(200, 200), ci::ColorA(1, 0, 0, 1));
				drawRect(ci::vec2(300, -100), ci::vec2(200, 200), ci::ColorA(1, 0, 0, 1));
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
