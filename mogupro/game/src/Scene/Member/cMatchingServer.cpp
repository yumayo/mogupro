#include <Scene/Member/cMatchingServer.h>
#include <Network/cMatchingMemberManager.h>
using namespace Network;
using namespace Network::Packet::Event;
using namespace Network::Packet::Request;
using namespace Network::Packet::Response;
namespace Scene
{
	namespace Member
	{
		void cMatchingServer::setup()
		{
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mOpenRoom = false;
		}
		void cMatchingServer::shutDown()
		{

		}

		void cMatchingServer::update(float deltaTime)
		{
			checkReqMakeRoom();
			checkReqInRoom();
		}

		void cMatchingServer::checkReqMakeRoom()
		{
			while (!cRequestManager::getInstance()->mReqMakeRoom.empty())
			{
				auto mReqMakeRoom = cRequestManager::getInstance()->mReqMakeRoom.top();
				if (mOpenRoom != false)
				{
					cUDPManager::getInstance()->send(mReqMakeRoom.mNetworkHandle, new cResMakeRoom(false));
					cRequestManager::getInstance()->mReqMakeRoom.pop();
					continue;
				}

				mOpenRoom = true;
				mRoomID = mReqMakeRoom.mRoomID;
				cMatchingMemberManager::getInstance()->mMasterHandle = mReqMakeRoom.mNetworkHandle;
				cUDPManager::getInstance()->send(mReqMakeRoom.mNetworkHandle, new cResMakeRoom(true));
				cMatchingMemberManager::getInstance()->addRoomMembers(mReqMakeRoom.mNetworkHandle);
				cRequestManager::getInstance()->mReqMakeRoom.pop();
				mPhaseState = PhaseState::IN_ROOM;
				continue;
			}
		}

		void cMatchingServer::checkReqInRoom()
		{
			while (!cRequestManager::getInstance()->mReqInRoom.empty())
			{
				auto reqInRoom = cRequestManager::getInstance()->mReqInRoom.top();
				if (mOpenRoom != true ||
					cMatchingMemberManager::getInstance()->addRoomMembers(reqInRoom.mNetworkHandle) != true)
				{
					cUDPManager::getInstance()->send(reqInRoom.mNetworkHandle, new cResInRoom(false));
					cRequestManager::getInstance()->mReqInRoom.pop();
					continue;
				}

				cUDPManager::getInstance()->send(reqInRoom.mNetworkHandle, new cResInRoom(true));
				cRequestManager::getInstance()->mReqInRoom.pop();
				continue;
			}

		}

		void cMatchingServer::draw()
		{

		}

		void cMatchingServer::draw2D()
		{

		}

		void cMatchingServer::resize()
		{

		}
	}
}
