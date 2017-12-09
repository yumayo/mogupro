#include <Scene/Member/cMatchingServer.h>
#include <Network/cMatchingMemberManager.h>
#include <Node/renderer.hpp>
#include <Node/action.hpp>
#include <Scene/Member/cGameMain.h>
#include <Scene/cSceneManager.h>
#include <CameraManager/cCameraManager.h>
#include <Utility/cInput.h>
#include <Game/cServerAdapter.h>
#include <boost/date_time/gregorian/gregorian.hpp>
#include <boost/date_time.hpp>
#include <cinder/Rand.h>
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
			mStartGame = false;
			teamCount = 0;
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
				n->remove_all_actions();
				mCanUpdateServerAdapter = true;
				mIsGameUpdate = true;
				mGameStartTime = boost::posix_time::second_clock::universal_time();
				mGameStartTime += boost::posix_time::seconds(3);
				mTimeStr = boost::posix_time::to_iso_string(mGameStartTime);

				//ゲーム開始時間のTimerSet
				cUDPServerManager::getInstance( )->broadcast( new cResSetGamestartTimer( mTimeStr ) );

				// 一旦サーバーとのタイマー同期を凍結します。2017/12/09 yumayo
				//n->run_action(Node::Action::repeat_forever::create(Node::Action::sequence::create(Node::Action::delay::create(0.5f),
				//	Node::Action::call_func::create([this]
				//{
				//	for each(auto m in cMatchingMemberManager::getInstance()->mPlayerDatas)
				//	{
				//		for each(auto p in cMatchingMemberManager::getInstance()->mPlayerDatas)
				//		{
				//			cUDPServerManager::getInstance()->send(m.networkHandle,
				//				new cResSetGamestartTimer(mTimeStr));
				//		}
				//	}
				//}))));

			}
			if (mIsGameUpdate)
			{
				checkStartGameMember();
				Game::cServerAdapter::getInstance()->update();
			}
		}

		void cMatchingServer::checkStartGameMember()
		{
			if (mStartGame != false)return;

			while (auto reqEndStartTimer = cRequestManager::getInstance()->getReqEndStartTimer())
			{
				for (int i = 0; i < cMatchingMemberManager::getInstance()->mPlayerDatas.size(); ++i)
				{
					if (reqEndStartTimer->mNetworkHandle
						!= cMatchingMemberManager::getInstance()->mPlayerDatas[i].networkHandle)
						continue;
					cMatchingMemberManager::getInstance()->mPlayerDatas[i].startGame = true;
				}
			}

			for each(auto m in cMatchingMemberManager::getInstance()->mPlayerDatas)
			{
				if (m.startGame == false)
					return;
			}

			mStartGame = true;
			n->remove_all_actions();
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
				//int teamNum = ci::randInt(2);
				int teamNum = teamCount % 2;
				++teamCount;
 				if (cMatchingMemberManager::getInstance()->changeTeamNum(teamNum,
					reqWantTeamIn->mNetworkHandle) != true)
				{
					cUDPServerManager::getInstance()->send(reqWantTeamIn->mNetworkHandle, new cResWantTeamIn(0, teamNum));
					continue;
				}
			
				cUDPServerManager::getInstance()->send(reqWantTeamIn->mNetworkHandle, new cResWantTeamIn(1, teamNum));
				//新規追加したPlayerの情報取得
				std::string newPlayerStr;
				int newPlayerID;
				for each(auto& m in cMatchingMemberManager::getInstance()->mPlayerDatas)
				{
					if (m.networkHandle != reqWantTeamIn->mNetworkHandle)continue;
					newPlayerStr = m.nameStr;
					newPlayerID = m.playerID;
				}

				for each(auto& m in cMatchingMemberManager::getInstance()->mPlayerDatas)
				{
					if (m.networkHandle != reqWantTeamIn->mNetworkHandle)
					{
						//!@LookMe : マッチングがランダムになったから出来る事であって本来はできない
						//メンバーに新規Playerの送信						
						cUDPServerManager::getInstance()->send(m.networkHandle,
							new cEveTeamMember(teamNum, newPlayerStr, newPlayerID));
						//新規Playerに他の既存Playerの送信
						cUDPServerManager::getInstance()->send(reqWantTeamIn->mNetworkHandle,
							new cEveTeamMember(m.teamNum, m.nameStr, m.playerID));
					}
				}
			
			}
		}

		void cMatchingServer::checkBeginGame()
		{
			while (auto reqCheckBeginGame = cRequestManager::getInstance()->getReqCheckBeginGame())
			{
				//Masterじゃない人ははじく
				if (cMatchingMemberManager::getInstance()->checkMaster(reqCheckBeginGame->mNetworkHandle) != true)
					continue;

				mPhaseState = PhaseState::BEGIN_GAME;

				for each(auto m in cMatchingMemberManager::getInstance()->mPlayerDatas)
				{
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
			cUDPServerManager::getInstance()->close();
			cUDPServerManager::getInstance()->open();
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
