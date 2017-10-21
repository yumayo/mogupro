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
		if (member.ipAddress == addMember.ipAddress && member.port == addMember.port)
			return false;
	}

	mRoomMembers.push_back(addMember);
	return true;
}
}
