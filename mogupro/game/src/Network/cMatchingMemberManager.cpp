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
		if (member.first == addMember)
			return false;
	}

	mRoomMembers.insert(std::make_pair(addMember,-1));
	return true;
}

bool cMatchingMemberManager::checkInMembers(Network::cNetworkHandle member)
{
	for each(auto m in mRoomMembers)
	{
		if (m.first == member)
			return true;
	}
	return false;
}
bool cMatchingMemberManager::checkTeamIn(int teamNum, Network::cNetworkHandle addMember)
{
	auto m = mRoomMembers.find(addMember);

	int teamNums[2];
	teamNums[0] = 0;
	teamNums[1] = 0;
	for each(auto m in mRoomMembers)
	{
		if (m.second != -1)
			teamNums[m.second]++;
	}

	if (m != mRoomMembers.end())
	{
		if (teamNums[teamNum] > 3)
			return false;
		m->second = teamNum;
		return true;
	}
	return false;
}

bool cMatchingMemberManager::checkMaster(cNetworkHandle masterHandle)
{
	if (mMasterHandle == masterHandle)
		return true;

	return false;
}

void cMatchingMemberManager::addPlayerDatas(std::string playerStr, ubyte1 teamNum)
{
	for each(auto p in mPlayerDatas)
	{
		if (p.nameStr == playerStr)
		{
			p.teamNum = teamNum;
			return;
		}
	}

	mPlayerDatas.push_back(PlayerData(teamNum,playerStr, mPlayerDatas.size()));
}


void cMatchingMemberManager::addPlayerDatas(std::string playerStr, ubyte1 teamNum, ubyte1 playerID)
{
	for each(auto p in mPlayerDatas)
	{
		if (p.nameStr == playerStr)
		{
			p.teamNum = teamNum;
			return;
		}
	}

	mPlayerDatas.push_back(PlayerData(teamNum, playerStr, playerID));
}
}
