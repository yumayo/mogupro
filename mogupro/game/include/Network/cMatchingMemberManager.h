#pragma once
#include <Network.hpp>
#include <Utility/cSingletonAble.h>

//! @file MatchingMemberManager
//! @brief MatchingMember‚ğæ“¾‚Å‚«‚Ü‚·
//! @note 
//!      ¡Œã‚±‚ê‚ğ•¡”‚É‚·‚éê‡‚ª‚ ‚é‚©‚à‚µ‚ê‚È‚¢
//! @date 2017-10-20
//! @author Taka Nomoto

namespace Network
{
class cMatchingMemberManager : public Utility::cSingletonAble<cMatchingMemberManager>
{
public:
	cMatchingMemberManager();
	~cMatchingMemberManager();
	bool addRoomMembers(Network::cNetworkHandle addMember);
	cNetworkHandle mMasterHandle;
	std::vector<cNetworkHandle> mRoomMembers;
	//‚±‚ê•ª‚¯‚é•K—v‚ª‚ ‚é‚Ì‚©
	std::vector<cNetworkHandle> m1PMembers;
	std::vector<cNetworkHandle> m2PMembers;

};
}
