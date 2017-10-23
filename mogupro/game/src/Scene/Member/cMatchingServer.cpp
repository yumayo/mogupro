#include <Scene/Member/cMatchingServer.h>
#include <Network/cMatchingMemberManager.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <Scene/Member/cGameMain.h>
#include <Scene/cSceneManager.h>
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
			using namespace Node::Action;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mOpenRoom = false;
			font = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 20);
			font->set_text(u8"‚Ê‚í‚ ‚ ‚Ÿ‚Ÿ‚Ÿ‚Ÿ‚ñ");
			font->set_scale(ci::vec2(1, -1));
			n->add_child(font);
			n->run_action(repeat_forever::create(sequence::create(delay::create(1.5f),
				call_func::create([this]
			{
				for each(auto m in cMatchingMemberManager::getInstance()->mRoomMembers)
				{
					for each(auto p in cMatchingMemberManager::getInstance()->mPlayerDatas)
					{
						cUDPServerManager::getInstance()->send(m.first,
							new cEveTeamMember(p.teamNum,p.nameStr,p.playerID));
					}
				}
			}))));
		}
		void cMatchingServer::shutDown()
		{

		}

		void cMatchingServer::update(float deltaTime)
		{
			n->entry_update(deltaTime);
			checkReqMakeRoom();
			checkReqInRoom();
			checkTeamIn();
			checkBeginGame();
		}

		void cMatchingServer::checkReqMakeRoom()
		{
			if (mPhaseState != PhaseState::NOT_IN_ROOM)
				return;

			while (!cRequestManager::getInstance()->mReqMakeRoom.empty())
			{
				auto mReqMakeRoom = cRequestManager::getInstance()->mReqMakeRoom.top();
				if (mOpenRoom != false)
				{
                    cUDPServerManager::getInstance()->send(mReqMakeRoom.mNetworkHandle, new cResMakeRoom(false));
					cRequestManager::getInstance()->mReqMakeRoom.pop();
					continue;
				}

				mOpenRoom = true;
				mRoomID = mReqMakeRoom.mRoomID;
				cMatchingMemberManager::getInstance()->mMasterHandle = mReqMakeRoom.mNetworkHandle;
                cUDPServerManager::getInstance()->send(mReqMakeRoom.mNetworkHandle, new cResMakeRoom(true));
				cMatchingMemberManager::getInstance()->addRoomMembers(mReqMakeRoom.mNetworkHandle);
				cRequestManager::getInstance()->mReqMakeRoom.pop();
				cMatchingMemberManager::getInstance()->addPlayerDatas("Mogura" + mReqMakeRoom.mNetworkHandle.ipAddress, -1);

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
                    cUDPServerManager::getInstance()->send(reqInRoom.mNetworkHandle, new cResInRoom(false));
					cRequestManager::getInstance()->mReqInRoom.pop();
					continue;
				}

                cUDPServerManager::getInstance()->send(reqInRoom.mNetworkHandle, new cResInRoom(true));
				cMatchingMemberManager::getInstance()->addPlayerDatas("Mogura" + reqInRoom.mNetworkHandle.ipAddress,-1);
				cRequestManager::getInstance()->mReqInRoom.pop();
				continue;
			}

		}

		void cMatchingServer::checkTeamIn()
		{
			while (!cRequestManager::getInstance()->mReqWantTeamIn.empty())
			{
				auto reqWantTeamIn = cRequestManager::getInstance()->mReqWantTeamIn.top();
				if (cMatchingMemberManager::getInstance()->checkTeamIn(reqWantTeamIn.mTeamNum,
					reqWantTeamIn.mNetworkHandle) != true)
				{
                    cUDPServerManager::getInstance()->send(reqWantTeamIn.mNetworkHandle, new cResWantTeamIn(0,reqWantTeamIn.mTeamNum));
					cRequestManager::getInstance()->mReqWantTeamIn.pop();
					continue;
				}
				
                cUDPServerManager::getInstance()->send(reqWantTeamIn.mNetworkHandle, new cResWantTeamIn(1, reqWantTeamIn.mTeamNum));
				cMatchingMemberManager::getInstance()->addPlayerDatas("Mogura" + reqWantTeamIn.mNetworkHandle.ipAddress, reqWantTeamIn.mTeamNum);
				cRequestManager::getInstance()->mReqWantTeamIn.pop();
				continue;
			}
		}

		void cMatchingServer::checkBeginGame()
		{
			while (!cRequestManager::getInstance()->mReqCheckBeginGame.empty())
			{
				auto reqCheckBeginGame = cRequestManager::getInstance()->mReqCheckBeginGame.top();
				//Master‚¶‚á‚È‚¢l‚Í‚Í‚¶‚­
				if (cMatchingMemberManager::getInstance()->checkMaster(reqCheckBeginGame.mNetworkHandle) != true)
				{
					cRequestManager::getInstance()->mReqWantTeamIn.pop();
					continue;
				}

				mPhaseState = PhaseState::BEGIN_GAME;

				for each(auto m in cMatchingMemberManager::getInstance()->mRoomMembers)
                    cUDPServerManager::getInstance()->send(m.first, new cResCheckBeginGame());
				
				cRequestManager::getInstance()->mReqWantTeamIn.pop();
				shutDown();
				cSceneManager::getInstance()->change<Scene::Member::cGameMain>();
				cSceneManager::getInstance()->now().setup();
				continue;
			}
		}

		void cMatchingServer::draw()
		{

		}

		void cMatchingServer::draw2D()
		{
			int c = 0;
			for each(auto m in cMatchingMemberManager::getInstance()->mRoomMembers)
			{
				font->set_text(u8"" + m.first.ipAddress + " : " + std::to_string(m.first.port));
				font->set_position_3d(glm::vec3(0, c * 50,0));
				n->entry_render(ci::mat4());
				c++;
			}
		}

		void cMatchingServer::resize()
		{

		}
	}
}
