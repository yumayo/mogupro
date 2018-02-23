#include <Network/cMatchingMemberManager.h>
#include <algorithm>
namespace Network
{
cMatchingMemberManager::cMatchingMemberManager( )
{
	teamCount[0] = 0;
	teamCount[1] = 0;

	mEmptyRedTeamIds = { { 0, false }, { 1, false }, { 2, false } };
	mEmptyBlueTeamIds = { { 4, false }, { 5, false }, { 6, false } };

	mNameStrs[0] = u8"もぐ太";
	mNameStrs[1] = u8"もぐ美";
	mNameStrs[2] = u8"卍もぐ卍";
	mNameStrs[3] = u8"もぐ助";
	mNameStrs[4] = u8"もぐぽよ";
	mNameStrs[5] = u8"もぐシェイク";

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

	if ( ( playerCount % 2 ) == 0 )
	{
		// 観戦者のIDが間に入ってきたのでsizeをIDにすることができなくなりました。yumayo
		auto& itr = std::find_if( mEmptyRedTeamIds.begin( ), mEmptyRedTeamIds.end( ), [ ] ( std::pair<int/*playerID*/, bool/*IDが埋まったかどうか*/> const& p ) { return p.second == false; } );
		if ( itr != mEmptyRedTeamIds.end( ) )
		{
			mPlayerDatas.push_back( PlayerData( 0, "Mogura" + addMember.ipAddress, itr->first, addMember ) );
			itr->second = true;
			playerCount++;
			return true;
		}
		else
		{
			return false;
		}
	}
	else
	{
		// 観戦者のIDが間に入ってきたのでsizeをIDにすることができなくなりました。yumayo
		auto& itr = std::find_if( mEmptyBlueTeamIds.begin( ), mEmptyBlueTeamIds.end( ), [ ] ( std::pair<int/*playerID*/, bool/*IDが埋まったかどうか*/> const& p ) { return p.second == false; } );
		if ( itr != mEmptyBlueTeamIds.end( ) )
		{
			mPlayerDatas.push_back( PlayerData( 1, "Mogura" + addMember.ipAddress, itr->first, addMember ) );
			itr->second = true;
			playerCount++;
			return true;
		}
		else
		{
			return false;
		}
	}
}

bool cMatchingMemberManager::addRoomMembersWatching( Network::cNetworkHandle addMember )
{
	if ( (int)mPlayerDatas.size( ) > 8 )return false;

	for each( auto member in mPlayerDatas )
	{
		if ( member.networkHandle == addMember )
			return false;
	}

	bool p3 = std::any_of( mPlayerDatas.begin( ), mPlayerDatas.end( ), [ ] ( PlayerData const& d ) { return d.playerID == 3; } );
	bool p7 = std::any_of( mPlayerDatas.begin( ), mPlayerDatas.end( ), [ ] ( PlayerData const& d ) { return d.playerID == 7; } );

	if ( !p3 )
	{
		mPlayerDatas.push_back( PlayerData( 0, "Mogura" + addMember.ipAddress, 3, addMember ) );
		return true;
	}
	else if ( !p7 )
	{

		mPlayerDatas.push_back( PlayerData( 1, "Mogura" + addMember.ipAddress, 7, addMember ) );
		return true;
	}
	else
	{
		// これ以上の観戦者は認められない。
		return false;
	}
}

// 決め打ちにすることにしました。
//bool cMatchingMemberManager::changeTeamNum(int teamNum, Network::cNetworkHandle member)
//{
//	if (teamCount[teamNum] > 4/*観戦者分増えました。yumayo*/)	
//		return false;
//
//	for (int i = 0; i < mPlayerDatas.size(); ++i)
//	{
//		if (mPlayerDatas[i].networkHandle != member)
//			continue;
//
//		mPlayerDatas[i].teamNum = teamNum;
//		int id = teamCount[teamNum];
//		teamCount[teamNum]++;
//		if (teamNum == 1)
//			id += 4;
//		// addRoomMembersでちゃんと決めたので大丈夫です。yumayo
////		mPlayerDatas[i].playerID = id;
//		return true;
//	}
//	return false;
//}

bool cMatchingMemberManager::checkInMembers(Network::cNetworkHandle member)
{
	for (auto& m : mPlayerDatas)
	{
		if (m.networkHandle == member)
			return true;
	}
	return false;
}

int cMatchingMemberManager::whatTeam( Network::cNetworkHandle handle )
{
	for ( auto& m : mPlayerDatas )
	{
		if ( m.networkHandle == handle )
			return m.teamNum;
	}
	return -1;
}

bool cMatchingMemberManager::checkMaster(cNetworkHandle masterHandle)
{
	if (mMasterHandle == masterHandle)
		return true;

	return false;
}

// この関数が呼ばれたら怖いため封印 yumayo
//void cMatchingMemberManager::addPlayerDatas(std::string playerStr, ubyte1 teamNum,cNetworkHandle networkHandle)
//{
//	for (int i = 0; i < mPlayerDatas.size(); ++i)
//	{
//		if (mPlayerDatas[i].nameStr == playerStr)
//		{
//			mPlayerDatas[i].teamNum = teamNum;
//			return;
//		}
//	}
//	mPlayerDatas.push_back(PlayerData(teamNum,playerStr, mPlayerDatas.size(),networkHandle));
//}


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

	// 観戦者が増えたため yumayo
	//if (mPlayerDatas.size() > 6)
	//	return;

	mPlayerDatas.push_back(PlayerData(teamNum, playerStr, playerID,networkHandle));
}

void cMatchingMemberManager::reset()
{
	mPlayerDatas.clear();
	mEmptyRedTeamIds.clear();
	mEmptyBlueTeamIds.clear();

	mPlayerID = 0;
	mPlayerTeamNum = 0;
	for(int i = 0; i < 2;i++)
	teamCount[i] = 0;
	playerCount = 0;
}
}
