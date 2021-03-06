#include <Scene/Member/cMatchingServer.h>
#include <Network/cMatchingMemberManager.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <Scene/Member/cGameMain.h>
#include <Scene/cSceneManager.h>
#include <CameraManager/cCameraManager.h>
#include <Utility/cInput.h>
#include <Game/cServerAdapter.h>
#include <cinder/Rand.h>
#include <Log/Log.h>

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
			CAMERA->setup();
			cUDPServerManager::getInstance()->open();
			using namespace Node::Action;
			mPhaseState = PhaseState::NOT_IN_ROOM;
			mOpenRoom = false;
			mIsGameUpdate = false;
			font = Node::Renderer::label::create("sawarabi-gothic-medium.ttf", 20);
			font->set_text(u8"ぬわああぁぁぁぁん");
			font->set_scale(glm::vec2(1, -1));
			n = Node::node::create();

			n->add_child(font);
			mStartGame = false;
			teamCount = 0;
		}
		void cMatchingServer::shutDown()
		{
			Game::cServerAdapter::getInstance()->removeInstance();
			cMatchingMemberManager::removeInstance();
			cUDPServerManager::getInstance()->close();
			cUDPServerManager::removeInstance();
		}

		void cMatchingServer::update(float deltaTime)
		{
			cUDPServerManager::getInstance()->update(deltaTime);

			updateServer(deltaTime);
			checkReqMakeRoom();
			checkReqInRoom();
			checkReqInRoomWatching( ); // 2017/12/14 yumayo
			checkTeamIn();
			checkBeginGame();
			resetMember();

			n->entry_update(deltaTime);
		}

		void cMatchingServer::updateServer(float deltaTime)
		{
			if ( cMatchingMemberManager::getInstance( )->mPlayerDatas.empty( ) ) return;

			if ( !mIsGameUpdate )
			{
				for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
				while ( auto reqEndGamemainSetup = m->ReqEndGamemainSetup.get( ) )
				{
					for ( int i = 0; i < cMatchingMemberManager::getInstance( )->mPlayerDatas.size( ); ++i )
					{
						if ( reqEndGamemainSetup->mNetworkHandle != cMatchingMemberManager::getInstance( )->mPlayerDatas[i].networkHandle )
							continue;
						cMatchingMemberManager::getInstance( )->mPlayerDatas[i].canUpdate = true;
					}
				}

				for ( auto m : cMatchingMemberManager::getInstance( )->mPlayerDatas )
				{
					if ( m.canUpdate == false )
						return;
				}
				n->remove_all_actions( );
				mIsGameUpdate = true;

				cUDPServerManager::getInstance( )->broadcast( new cResSetGamestartTimer( Network::cUDPServerManager::getInstance()->getServerTime( ) + 2.0F ) );
			}
			else
			{
				checkStartGameMember();
				Game::cServerAdapter::getInstance()->update();
			}
		}

		void cMatchingServer::checkStartGameMember()
		{
			if (mStartGame != false) return;

			for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
			while (auto reqEndStartTimer = m->ReqEndStartTimer.get())
			{
				for (int i = 0; i < cMatchingMemberManager::getInstance()->mPlayerDatas.size(); ++i)
				{
					if (reqEndStartTimer->mNetworkHandle
						!= cMatchingMemberManager::getInstance()->mPlayerDatas[i].networkHandle)
						continue;
					cMatchingMemberManager::getInstance()->mPlayerDatas[i].startGame = true;
				}
			}

			for (auto m : cMatchingMemberManager::getInstance()->mPlayerDatas)
			{
				if (m.startGame == false)
					return;
			}

			mStartGame = true;
			n->remove_all_actions();
		}

		void cMatchingServer::checkReqMakeRoom()
		{
			for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
			while (auto mReqMakeRoom = m->ReqMakeRoom.get())
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
			for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
			while (auto reqInRoom = m->ReqInRoom.get())
			{
				if (mOpenRoom != true || mPhaseState != PhaseState::IN_ROOM ||
					cMatchingMemberManager::getInstance()->addRoomMembers(reqInRoom->mNetworkHandle) != true)
				{
					cUDPServerManager::getInstance()->send(reqInRoom->mNetworkHandle, new cResInRoom(false));
					continue;
				}
				cUDPServerManager::getInstance()->send(reqInRoom->mNetworkHandle, new cResInRoom(true));
			}

		}

		void cMatchingServer::checkReqInRoomWatching( )
		{
			for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
			while ( auto reqInRoom = m->ReqInRoomWatching.get( ) )
			{
				if ( mOpenRoom != true || mPhaseState != PhaseState::IN_ROOM ||
					 cMatchingMemberManager::getInstance( )->addRoomMembersWatching( reqInRoom->networkHandle ) != true )
				{
					cUDPServerManager::getInstance( )->send( reqInRoom->networkHandle, new cResInRoom( false ) );
					continue;
				}
				cUDPServerManager::getInstance( )->send( reqInRoom->networkHandle, new cResInRoom( true ) );
			}

		}

		void cMatchingServer::checkTeamIn()
		{
			for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
			while (auto reqWantTeamIn = m->ReqWantTeamIn.get())
			{
				auto team = cMatchingMemberManager::getInstance( )->whatTeam( reqWantTeamIn->mNetworkHandle );
 				if (cMatchingMemberManager::getInstance()->whatTeam( reqWantTeamIn->mNetworkHandle) == -1)
				{
					cUDPServerManager::getInstance()->send(reqWantTeamIn->mNetworkHandle, new cResWantTeamIn(0, -1,0));
					continue;
				}

				++teamCount;

				//新規追加したPlayerの情報取得
				std::string newPlayerStr;
				int newPlayerID;
				for (auto& m : cMatchingMemberManager::getInstance()->mPlayerDatas)
				{
					if (m.networkHandle != reqWantTeamIn->mNetworkHandle)continue;
					newPlayerStr = m.nameStr;
					newPlayerID = m.playerID;
				}

				cUDPServerManager::getInstance()->send(reqWantTeamIn->mNetworkHandle, new cResWantTeamIn(1, team ,newPlayerID));
				
				int count = 0;
				for (auto& m : cMatchingMemberManager::getInstance()->mPlayerDatas)
				{
					if (m.networkHandle != reqWantTeamIn->mNetworkHandle)
					{
						//!@LookMe : マッチングがランダムになったから出来る事であって本来はできない
						//メンバーに新規Playerの送信						
						cUDPServerManager::getInstance()->send(m.networkHandle,
							new cEveTeamMember( team, newPlayerStr, newPlayerID));
						//新規Playerに他の既存Playerの送信
						cUDPServerManager::getInstance()->send(reqWantTeamIn->mNetworkHandle,
							new cEveTeamMember(m.teamNum, m.nameStr, m.playerID));
						++count;
					}
				}
			
				ci::app::console() << "MemberCount : " << count << std::endl;
				Log::cLogManager::getInstance()->add("MatchingServer");
				Log::cLogManager::getInstance()->writeLog("MatchingServer", "MatchingMember : " + std::to_string(count));
			}
		}

		void cMatchingServer::checkBeginGame()
		{
			for ( auto& m : Network::cUDPServerManager::getInstance( )->getUDPManager( ) )
			while (auto reqCheckBeginGame = m->ReqCheckBeginGame.get())
			{
				//Masterじゃない人ははじく
				if (cMatchingMemberManager::getInstance()->checkMaster(reqCheckBeginGame->mNetworkHandle) != true)
					continue;

				mPhaseState = PhaseState::BEGIN_GAME;

				for (auto m : cMatchingMemberManager::getInstance()->mPlayerDatas)
				{
					cUDPServerManager::getInstance()->send(m.networkHandle, new cResCheckBeginGame(m.playerID));
				}
			}
		}

		void cMatchingServer::resetMember()
		{
			bool isReset = false;
			for (auto& m : Network::cUDPServerManager::getInstance()->getUDPManager())
			while (auto req = m->ReqGameEnd.get())
			{
				isReset = true;
			}
			if (ENV->pushKey(ci::app::KeyEvent::KEY_SPACE))
			{
				isReset = true;
			}
			if(isReset)
			{
				mPhaseState = PhaseState::NOT_IN_ROOM;
				mOpenRoom = false;
				mIsGameUpdate = false;
				Game::cServerAdapter::getInstance()->removeInstance();
				cMatchingMemberManager::removeInstance();
				cUDPServerManager::getInstance()->close();
				cUDPServerManager::getInstance()->open();
			}
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
