#include <Network/cResponseManager.h>
#include <cinder/app/App.h>
namespace Network
{
// P=====BEGIN=====P
boost::optional<Packet::Response::cResString> cResponseManager::getResString( )
{
    if ( mResString.empty( ) )
    {
        auto it = mResStringSequenceIds.begin( );
		while ( it != mResStringSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResStringSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResString.top( );
        mResString.pop( );
        return top;
    }
}
void cResponseManager::ungetResString( Packet::Response::cResString&& data )
{
    mResString.push( std::move( data ) );
}
bool cResponseManager::isNewResString( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResStringSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResConnect> cResponseManager::getResConnect( )
{
    if ( mResConnect.empty( ) )
    {
        auto it = mResConnectSequenceIds.begin( );
		while ( it != mResConnectSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResConnectSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResConnect.top( );
        mResConnect.pop( );
        return top;
    }
}
void cResponseManager::ungetResConnect( Packet::Response::cResConnect&& data )
{
    mResConnect.push( std::move( data ) );
}
bool cResponseManager::isNewResConnect( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResConnectSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResGetJemSeed> cResponseManager::getResGetJemSeed( )
{
    if ( mResGetJemSeed.empty( ) )
    {
        auto it = mResGetJemSeedSequenceIds.begin( );
		while ( it != mResGetJemSeedSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResGetJemSeedSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResGetJemSeed.top( );
        mResGetJemSeed.pop( );
        return top;
    }
}
void cResponseManager::ungetResGetJemSeed( Packet::Response::cResGetJemSeed&& data )
{
    mResGetJemSeed.push( std::move( data ) );
}
bool cResponseManager::isNewResGetJemSeed( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResGetJemSeedSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResGetJemPoint> cResponseManager::getResGetJemPoint( )
{
    if ( mResGetJemPoint.empty( ) )
    {
        auto it = mResGetJemPointSequenceIds.begin( );
		while ( it != mResGetJemPointSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResGetJemPointSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResGetJemPoint.top( );
        mResGetJemPoint.pop( );
        return top;
    }
}
void cResponseManager::ungetResGetJemPoint( Packet::Response::cResGetJemPoint&& data )
{
    mResGetJemPoint.push( std::move( data ) );
}
bool cResponseManager::isNewResGetJemPoint( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResGetJemPointSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResCheckGetJemPlayer> cResponseManager::getResCheckGetJemPlayer( )
{
    if ( mResCheckGetJemPlayer.empty( ) )
    {
        auto it = mResCheckGetJemPlayerSequenceIds.begin( );
		while ( it != mResCheckGetJemPlayerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResCheckGetJemPlayerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResCheckGetJemPlayer.top( );
        mResCheckGetJemPlayer.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckGetJemPlayer( Packet::Response::cResCheckGetJemPlayer&& data )
{
    mResCheckGetJemPlayer.push( std::move( data ) );
}
bool cResponseManager::isNewResCheckGetJemPlayer( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResCheckGetJemPlayerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResCheckGetJemQuarry> cResponseManager::getResCheckGetJemQuarry( )
{
    if ( mResCheckGetJemQuarry.empty( ) )
    {
        auto it = mResCheckGetJemQuarrySequenceIds.begin( );
		while ( it != mResCheckGetJemQuarrySequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResCheckGetJemQuarrySequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResCheckGetJemQuarry.top( );
        mResCheckGetJemQuarry.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckGetJemQuarry( Packet::Response::cResCheckGetJemQuarry&& data )
{
    mResCheckGetJemQuarry.push( std::move( data ) );
}
bool cResponseManager::isNewResCheckGetJemQuarry( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResCheckGetJemQuarrySequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResCheckPlayerRobJem> cResponseManager::getResCheckPlayerRobJem( )
{
    if ( mResCheckPlayerRobJem.empty( ) )
    {
        auto it = mResCheckPlayerRobJemSequenceIds.begin( );
		while ( it != mResCheckPlayerRobJemSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResCheckPlayerRobJemSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResCheckPlayerRobJem.top( );
        mResCheckPlayerRobJem.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckPlayerRobJem( Packet::Response::cResCheckPlayerRobJem&& data )
{
    mResCheckPlayerRobJem.push( std::move( data ) );
}
bool cResponseManager::isNewResCheckPlayerRobJem( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResCheckPlayerRobJemSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResCheckSetQuarry> cResponseManager::getResCheckSetQuarry( )
{
    if ( mResCheckSetQuarry.empty( ) )
    {
        auto it = mResCheckSetQuarrySequenceIds.begin( );
		while ( it != mResCheckSetQuarrySequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResCheckSetQuarrySequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResCheckSetQuarry.top( );
        mResCheckSetQuarry.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckSetQuarry( Packet::Response::cResCheckSetQuarry&& data )
{
    mResCheckSetQuarry.push( std::move( data ) );
}
bool cResponseManager::isNewResCheckSetQuarry( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResCheckSetQuarrySequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResCheckPlayerDeath> cResponseManager::getResCheckPlayerDeath( )
{
    if ( mResCheckPlayerDeath.empty( ) )
    {
        auto it = mResCheckPlayerDeathSequenceIds.begin( );
		while ( it != mResCheckPlayerDeathSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResCheckPlayerDeathSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResCheckPlayerDeath.top( );
        mResCheckPlayerDeath.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckPlayerDeath( Packet::Response::cResCheckPlayerDeath&& data )
{
    mResCheckPlayerDeath.push( std::move( data ) );
}
bool cResponseManager::isNewResCheckPlayerDeath( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResCheckPlayerDeathSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResMakeRoom> cResponseManager::getResMakeRoom( )
{
    if ( mResMakeRoom.empty( ) )
    {
        auto it = mResMakeRoomSequenceIds.begin( );
		while ( it != mResMakeRoomSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResMakeRoomSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResMakeRoom.top( );
        mResMakeRoom.pop( );
        return top;
    }
}
void cResponseManager::ungetResMakeRoom( Packet::Response::cResMakeRoom&& data )
{
    mResMakeRoom.push( std::move( data ) );
}
bool cResponseManager::isNewResMakeRoom( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResMakeRoomSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResInRoom> cResponseManager::getResInRoom( )
{
    if ( mResInRoom.empty( ) )
    {
        auto it = mResInRoomSequenceIds.begin( );
		while ( it != mResInRoomSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResInRoomSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResInRoom.top( );
        mResInRoom.pop( );
        return top;
    }
}
void cResponseManager::ungetResInRoom( Packet::Response::cResInRoom&& data )
{
    mResInRoom.push( std::move( data ) );
}
bool cResponseManager::isNewResInRoom( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResInRoomSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResWantTeamIn> cResponseManager::getResWantTeamIn( )
{
    if ( mResWantTeamIn.empty( ) )
    {
        auto it = mResWantTeamInSequenceIds.begin( );
		while ( it != mResWantTeamInSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResWantTeamInSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResWantTeamIn.top( );
        mResWantTeamIn.pop( );
        return top;
    }
}
void cResponseManager::ungetResWantTeamIn( Packet::Response::cResWantTeamIn&& data )
{
    mResWantTeamIn.push( std::move( data ) );
}
bool cResponseManager::isNewResWantTeamIn( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResWantTeamInSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResCheckBeginGame> cResponseManager::getResCheckBeginGame( )
{
    if ( mResCheckBeginGame.empty( ) )
    {
        auto it = mResCheckBeginGameSequenceIds.begin( );
		while ( it != mResCheckBeginGameSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResCheckBeginGameSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResCheckBeginGame.top( );
        mResCheckBeginGame.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckBeginGame( Packet::Response::cResCheckBeginGame&& data )
{
    mResCheckBeginGame.push( std::move( data ) );
}
bool cResponseManager::isNewResCheckBeginGame( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResCheckBeginGameSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResCheckMember> cResponseManager::getResCheckMember( )
{
    if ( mResCheckMember.empty( ) )
    {
        auto it = mResCheckMemberSequenceIds.begin( );
		while ( it != mResCheckMemberSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResCheckMemberSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResCheckMember.top( );
        mResCheckMember.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckMember( Packet::Response::cResCheckMember&& data )
{
    mResCheckMember.push( std::move( data ) );
}
bool cResponseManager::isNewResCheckMember( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResCheckMemberSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResEndGamemainSetup> cResponseManager::getResEndGamemainSetup( )
{
    if ( mResEndGamemainSetup.empty( ) )
    {
        auto it = mResEndGamemainSetupSequenceIds.begin( );
		while ( it != mResEndGamemainSetupSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResEndGamemainSetupSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResEndGamemainSetup.top( );
        mResEndGamemainSetup.pop( );
        return top;
    }
}
void cResponseManager::ungetResEndGamemainSetup( Packet::Response::cResEndGamemainSetup&& data )
{
    mResEndGamemainSetup.push( std::move( data ) );
}
bool cResponseManager::isNewResEndGamemainSetup( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResEndGamemainSetupSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
// P=====END=====P
}
