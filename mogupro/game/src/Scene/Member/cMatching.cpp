#include <Scene/Member/cMatching.h>
#include "cinder/gl/gl.h"
#include <Utility/cInput.h>
#include <string>
#include <Network.hpp>
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
	if (posA.x - sizeA.x / 2 > mousePos.x )return false;
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
	}

	void cMatching::shutDown()
	{

	}

	void cMatching::update(float deltaTime)
	{
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
				cUDPManager::getInstance()->send(cNetworkHandle("127.0.0.1", 25565), new cReqMakeRoom(100));
				mWaitClassState = ClassState::MASTER;
			}

			else if (ENV->pushKey(ci::app::KeyEvent::KEY_x))
			{
				cUDPManager::getInstance()->send(cNetworkHandle("127.0.0.1", 25565), new cReqInRoom(100));
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

	}

	void cMatching::draw()
	{

	}
	void cMatching::draw2D()
	{
		switch (mPhaseState)
		{
		case PhaseState::NOT_IN_ROOM:
			drawRect(ci::vec2(-300, -100), ci::vec2(200,100),ci::ColorA(1,0,0,1));
			drawRect(ci::vec2(300, -100), ci::vec2(200, 100), ci::ColorA(1, 0, 0, 1));
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
