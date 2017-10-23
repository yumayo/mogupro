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
	cNetworkHandle mMasterHandle;
	std::map<cNetworkHandle, int> mRoomMembers;
};
}
