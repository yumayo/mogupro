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
boost::optional<Packet::Response::cResSetGamestartTimer> cResponseManager::getResSetGamestartTimer( )
{
    if ( mResSetGamestartTimer.empty( ) )
    {
        auto it = mResSetGamestartTimerSequenceIds.begin( );
		while ( it != mResSetGamestartTimerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResSetGamestartTimerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResSetGamestartTimer.top( );
        mResSetGamestartTimer.pop( );
        return top;
    }
}
void cResponseManager::ungetResSetGamestartTimer( Packet::Response::cResSetGamestartTimer&& data )
{
    mResSetGamestartTimer.push( std::move( data ) );
}
bool cResponseManager::isNewResSetGamestartTimer( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResSetGamestartTimerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResCannonPower> cResponseManager::getResCannonPower( )
{
    if ( mResCannonPower.empty( ) )
    {
        auto it = mResCannonPowerSequenceIds.begin( );
		while ( it != mResCannonPowerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResCannonPowerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResCannonPower.top( );
        mResCannonPower.pop( );
        return top;
    }
}
void cResponseManager::ungetResCannonPower( Packet::Response::cResCannonPower&& data )
{
    mResCannonPower.push( std::move( data ) );
}
bool cResponseManager::isNewResCannonPower( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResCannonPowerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Response::cResResult> cResponseManager::getResResult( )
{
    if ( mResResult.empty( ) )
    {
        auto it = mResResultSequenceIds.begin( );
		while ( it != mResResultSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mResResultSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mResResult.top( );
        mResResult.pop( );
        return top;
    }
}
void cResponseManager::ungetResResult( Packet::Response::cResResult&& data )
{
    mResResult.push( std::move( data ) );
}
bool cResponseManager::isNewResResult( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mResResultSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
// P=====END=====P
}
