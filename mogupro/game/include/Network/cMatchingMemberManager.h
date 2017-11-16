#pragma once
#include <Network.hpp>
#include <Utility/cSingletonAble.h>

//! @file MatchingMemberManager
//! @brief MatchingMemberを取得できます
//! @note 
//!      今後これを複数にする場合があるかもしれない
//! @date 2017-10-20
//! @author Taka Nomoto

namespace Network
{
	struct PlayerData
	{
		ubyte1 teamNum;
		std::string nameStr;
		ubyte1 playerID;
		cNetworkHandle networkHandle;
		bool canUpdate;
		bool startGame;
		PlayerData()
		{

		}

		PlayerData(ubyte1 _teamNum, std::string _nameStr, ubyte1 _playerID, cNetworkHandle _networkHandle) :
			teamNum(_teamNum), nameStr(_nameStr), playerID(_playerID),networkHandle(_networkHandle), canUpdate(false), startGame(false)
		{

		}
	};

class cMatchingMemberManager : public Utility::cSingletonAble<cMatchingMemberManager>
{
public:
	cMatchingMemberManager();
	~cMatchingMemberManager();
	//メンバー追加
	bool addRoomMembers(Network::cNetworkHandle addMember);
	//メンバーにいるかどうか (知らない奴からの通信はじくため)
	bool checkInMembers(Network::cNetworkHandle member);
	//チーム番号の変更
	bool changeTeamNum(int teamNum, Network::cNetworkHandle member);
	//マスターかどうかの判断
	bool checkMaster(cNetworkHandle masterHandle);
	void addPlayerDatas(std::string playerStr,ubyte1 teamNum, cNetworkHandle networkHandle);
	void addPlayerDatas(std::string playerStr, ubyte1 teamNum, ubyte1 playerID,cNetworkHandle networkHandle);
	cNetworkHandle mMasterHandle;
	std::vector<PlayerData> mPlayerDatas;
    int mPlayerID = 0;
};
}
