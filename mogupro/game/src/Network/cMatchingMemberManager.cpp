#include <Network/cMatchingMemberManager.h>
namespace Network
{
cMatchingMemberManager::cMatchingMemberManager( ) :mMasterHandle("",0)
{

}
cMatchingMemberManager::~cMatchingMemberManager( )
{

}
bool cMatchingMemberManager::addRoomMembers(Network::cNetworkHandle addMember)
{
	if ((int)mRoomMembers.size() > 8)return false;

	for each(auto member in mRoomMembers)
	{
		if (member.first.ipAddress == addMember.ipAddress && member.first.port == addMember.port)
			return false;
	}

	mRoomMembers.insert(std::make_pair(addMember,-1));
	return true;
}

bool cMatchingMemberManager::checkInMembers(Network::cNetworkHandle member)
{
	for each(auto m in mRoomMembers)
	{
		if (m.first.ipAddress == member.ipAddress && m.first.port == member.port)
			return true;
	}
	return false;
}
bool cMatchingMemberManager::checkTeamIn(int teamNum, Network::cNetworkHandle addMember)
{
	auto m = mRoomMembers.find(addMember);

	if (m != mRoomMembers.end())
	{
		m->second = teamNum;
		return true;
	}
	return false;
}

bool cMatchingMemberManager::checkMaster(cNetworkHandle masterHandle)
{
	if (mMasterHandle.ipAddress == masterHandle.ipAddress
		&& mMasterHandle.port == masterHandle.port)
		return true;

	return false;
}
}
