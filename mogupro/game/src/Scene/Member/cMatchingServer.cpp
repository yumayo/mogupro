#include <Scene/Member/cMatchingServer.h>
#include <Network/cMatchingMemberManager.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <Scene/Member/cGameMain.h>
#include <Scene/cSceneManager.h>
#include <CameraManager/cCameraManager.h>
#include <Utility/cInput.h>
#include <Game/cServerAdapter.h>
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
			ci::vec3 pos(0, 0, 0);
			CAMERA->followingCamera(&pos, 30);
			CAMERA->setup();
			cUDPServerManager::getInstance()->open();
			using namespace Node::Action;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mOpenRoom = false;
            mIsGameUpdate = false;
			font = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 20);
			font->set_text(u8"‚Ê‚í‚ ‚ ‚Ÿ‚Ÿ‚Ÿ‚Ÿ‚ñ");
			font->set_scale(glm::vec2(1, -1));
			n = Node::node::create();

			n->add_child(font);
			n->run_action(repeat_forever::create(sequence::create(delay::create(1.5f),
				call_func::create([this]
			{
				for each(auto m in cMatchingMemberManager::getInstance()->mPlayerDatas)
				{
					for each(auto p in cMatchingMemberManager::getInstance()->mPlayerDatas)
					{
						cUDPServerManager::getInstance()->send(m.networkHandle,
							new cEveTeamMember(p.teamNum, p.nameStr, p.playerID));
					}
				}
			}))));
			mCanUpdateServerAdapter = false;
		}
		void cMatchingServer::shutDown()
		{

		}

		void cMatchingServer::update(float deltaTime)
		{
			cUDPServerManager::getInstance()->update(deltaTime);
			updateServer(deltaTime);
			n->entry_update(deltaTime);
			checkReqMakeRoom();
			checkReqInRoom();
			checkTeamIn();
			checkBeginGame();
			resetMember();
		}

		void cMatchingServer::updateServer(float deltaTime)
		{

			if (mCanUpdateServerAdapter == false)
			{
				while (auto reqEndGamemainSetup = cRequestManager::getInstance()->getReqEndGamemainSetup())
				{
					for (int i = 0; i < cMatchingMemberManager::getInstance()->mPlayerDatas.size(); ++i)
					{
						if (reqEndGamemainSetup->mNetworkHandle
							!= cMatchingMemberManager::getInstance()->mPlayerDatas[i].networkHandle)
							continue;
						cMatchingMemberManager::getInstance()->mPlayerDatas[i].canUpdate = true;
					}
				}

				for each(auto m in cMatchingMemberManager::getInstance()->mPlayerDatas)
				{
					if (m.canUpdate == false)
						return;
				}
				mCanUpdateServerAdapter = true;
				mIsGameUpdate = true;
			}
			if (mIsGameUpdate)
			{
				Game::cServerAdapter::getInstance()->update();
			}
		}

		void cMatchingServer::checkReqMakeRoom()
		{
			if (mPhaseState != PhaseState::NOT_IN_ROOM)
				return;

			while (auto mReqMakeRoom = cRequestManager::getInstance()->getReqMakeRoom())
			{
				if (mOpenRoom != false)
				{
					cUDPServerManager::getInstance()->send(mReqMakeRoom->mNetworkHandle, new cResMakeRoom(false));
					continue;
				}

				mOpenRoom = true;
				mRoomID = mReqMakeRoom->mRoomID;
				cMatchingMemberManager::getInstance()->mMasterHandle = mReqMakeRoom->mNetworkHandle;
				cUDPServerManager::getInstance()->send(mReqMakeRoom->mNetworkHandle, new cResMakeRoom(true));
				cMatchingMemberManager::getInstance()->addRoomMembers(mReqMakeRoom->mNetworkHandle);
				mPhaseState = PhaseState::IN_ROOM;
			}
		}

		void cMatchingServer::checkReqInRoom()
		{
			while (auto reqInRoom = cRequestManager::getInstance()->getReqInRoom())
			{
				if (mOpenRoom != true ||
					cMatchingMemberManager::getInstance()->addRoomMembers(reqInRoom->mNetworkHandle) != true)
				{
					cUDPServerManager::getInstance()->send(reqInRoom->mNetworkHandle, new cResInRoom(false));
					continue;
				}
				cUDPServerManager::getInstance()->send(reqInRoom->mNetworkHandle, new cResInRoom(true));
			}

		}

		void cMatchingServer::checkTeamIn()
		{
			while (auto reqWantTeamIn = cRequestManager::getInstance()->getReqWantTeamIn())
			{
				if (cMatchingMemberManager::getInstance()->changeTeamNum(reqWantTeamIn->mTeamNum,
					reqWantTeamIn->mNetworkHandle) != true)
				{
					cUDPServerManager::getInstance()->send(reqWantTeamIn->mNetworkHandle, new cResWantTeamIn(0, reqWantTeamIn->mTeamNum));
					continue;
				}
				cUDPServerManager::getInstance()->send(reqWantTeamIn->mNetworkHandle, new cResWantTeamIn(1, reqWantTeamIn->mTeamNum));
			}
		}

		void cMatchingServer::checkBeginGame()
		{
			while (auto reqCheckBeginGame = cRequestManager::getInstance()->getReqCheckBeginGame())
			{
				//Master‚¶‚á‚È‚¢l‚Í‚Í‚¶‚­
				if (cMatchingMemberManager::getInstance()->checkMaster(reqCheckBeginGame->mNetworkHandle) != true)
					continue;
			
				mPhaseState = PhaseState::BEGIN_GAME;
				for each(auto m in cMatchingMemberManager::getInstance()->mPlayerDatas)
				{
					for each(auto p in cMatchingMemberManager::getInstance()->mPlayerDatas)
					{
						cUDPServerManager::getInstance()->send(m.networkHandle,
							new cEveTeamMember(p.teamNum, p.nameStr, p.playerID));
					}
					cUDPServerManager::getInstance()->send(m.networkHandle, new cResCheckBeginGame(m.playerID));
				}
			}
		}

		void cMatchingServer::resetMember()
		{
			if (!ENV->pushKey(ci::app::KeyEvent::KEY_SPACE))return;
			
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mOpenRoom = false;
			mIsGameUpdate = false;
			cMatchingMemberManager::getInstance()->mPlayerDatas.clear();
			cMatchingMemberManager::getInstance()->mMasterHandle = cNetworkHandle();
			mCanUpdateServerAdapter = false;
		}
		void cMatchingServer::draw()
		{

		}

		void cMatchingServer::draw2D()
		{
			int c = 0;
			for each(auto m in cMatchingMemberManager::getInstance()->mPlayerDatas)
			{
				font->set_text(u8"" + m.networkHandle.ipAddress 
					+ " playerID : " + std::to_string(m.playerID) + " taemNum : " + std::to_string(m.teamNum));
				font->set_position_3d(glm::vec3(0, c * 50, 0));
				n->entry_render(ci::mat4());
				c++;
			}
		}

		void cMatchingServer::resize()
		{

		}
	}
}
