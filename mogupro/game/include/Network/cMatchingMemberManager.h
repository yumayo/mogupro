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
		
		PlayerData()
		{

		}
		PlayerData(ubyte1 _teamNum,std::string _nameStr,ubyte1 _playerID) : 
			teamNum(_teamNum),nameStr(_nameStr),playerID(_playerID)
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
	//Teamにメンバー追加 コードが汚いからリファクタリング必要
	bool checkTeamIn(int teamNum,Network::cNetworkHandle addMember);
	bool checkMaster(cNetworkHandle masterHandle);
	void addPlayerDatas(std::string playerStr,ubyte1 teamNum);
	void addPlayerDatas(std::string playerStr, ubyte1 teamNum, ubyte1 playerID);
	cNetworkHandle mMasterHandle;
	std::map<cNetworkHandle, int> mRoomMembers;
	std::vector<PlayerData> mPlayerDatas;
    int mPlayerID = 0;
};
}
