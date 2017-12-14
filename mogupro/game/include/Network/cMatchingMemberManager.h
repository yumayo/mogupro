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
	//メンバー追加 2017/12/14 yumayo 観戦用
	bool addRoomMembersWatching( Network::cNetworkHandle addMember );
	//メンバーにいるかどうか (知らない奴からの通信はじくため)
	bool checkInMembers(Network::cNetworkHandle member);
	//チーム番号の変更 2017/12/14 yumayo 決め打ちでチーム番号を決めます。
	//bool changeTeamNum(int teamNum, Network::cNetworkHandle member);
	int whatTeam( Network::cNetworkHandle handle );
	//マスターかどうかの判断
	bool checkMaster(cNetworkHandle masterHandle);
	// 観戦者が増えたため封印 yuamyo
	//void addPlayerDatas(std::string playerStr,ubyte1 teamNum, cNetworkHandle networkHandle);
	void addPlayerDatas(std::string playerStr, ubyte1 teamNum, ubyte1 playerID,cNetworkHandle networkHandle);
	cNetworkHandle mMasterHandle;
	std::vector<PlayerData> mPlayerDatas;
	std::map<int, bool> mEmptyRedTeamIds; // 2017/12/14 観戦者が入ってきてID管理が面倒になったため。
	std::map<int, bool> mEmptyBlueTeamIds; // 2017/12/14 観戦者が入ってきてID管理が面倒になったため。
    int mPlayerID = 0;
	int mPlayerTeamNum = 0;
private:
	int teamCount[2];
	int playerCount = 0;
};
}
