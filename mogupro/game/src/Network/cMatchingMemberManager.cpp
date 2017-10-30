#include <Network/cMatchingMemberManager.h>
namespace Network
{
cMatchingMemberManager::cMatchingMemberManager( )
{

}

cMatchingMemberManager::~cMatchingMemberManager( )
{

}

bool cMatchingMemberManager::addRoomMembers(Network::cNetworkHandle addMember)
{
	if ((int)mPlayerDatas.size() > 8)return false;

	for each(auto member in mPlayerDatas)
	{
		if (member.networkHandle == addMember)
			return false;
	}

	mPlayerDatas.push_back(PlayerData(-1,"Mogura" + addMember.ipAddress, mPlayerDatas.size(),addMember));
	return true;
}

bool cMatchingMemberManager::changeTeamNum(int teamNum, Network::cNetworkHandle member)
{
	int teamNums[2];
	teamNums[0] = 0;
	teamNums[1] = 0;
	for each(auto p in mPlayerDatas)
	{
		if (p.teamNum == 0 || p.teamNum == 1)
			teamNums[p.teamNum]++;
	}
	if (teamNums[teamNum] > 3)
		return false;

	for (int i = 0; i < mPlayerDatas.size(); ++i)
	{
		if (mPlayerDatas[i].networkHandle != member)
			continue;

		mPlayerDatas[i].teamNum = teamNum;
		return true;
	}
	return false;
}

bool cMatchingMemberManager::checkInMembers(Network::cNetworkHandle member)
{
	for each(auto m in mPlayerDatas)
	{
		if (m.networkHandle == member)
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

void cMatchingMemberManager::addPlayerDatas(std::string playerStr, ubyte1 teamNum,cNetworkHandle networkHandle)
{
	for (int i = 0; i < mPlayerDatas.size(); ++i)
	{
		if (mPlayerDatas[i].nameStr == playerStr)
		{
			mPlayerDatas[i].teamNum = teamNum;
			return;
		}
	}
	mPlayerDatas.push_back(PlayerData(teamNum,playerStr, mPlayerDatas.size(),networkHandle));
}


void cMatchingMemberManager::addPlayerDatas(std::string playerStr, ubyte1 teamNum, ubyte1 playerID, cNetworkHandle networkHandle)
{
	for (int i = 0; i < mPlayerDatas.size(); ++i)
	{
		if (mPlayerDatas[i].nameStr == playerStr)
		{
			mPlayerDatas[i].teamNum = teamNum;
			return;
		}
	}

	if (mPlayerDatas.size() > 8)
		return;

	mPlayerDatas.push_back(PlayerData(teamNum, playerStr, playerID,networkHandle));
}
}
