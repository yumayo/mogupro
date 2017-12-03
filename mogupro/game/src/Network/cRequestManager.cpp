#include <Network/cRequestManager.h>
#include <cinder/app/App.h>
namespace Network
{
// P=====BEGIN=====P
boost::optional<Packet::Request::cReqString> cRequestManager::getReqString( )
{
    if ( mReqString.empty( ) )
    {
        auto it = mReqStringSequenceIds.begin( );
		while ( it != mReqStringSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqStringSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqString.top( );
        mReqString.pop( );
        return top;
    }
}
void cRequestManager::ungetReqString( Packet::Request::cReqString&& data )
{
    mReqString.push( std::move( data ) );
}
bool cRequestManager::isNewReqString( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqStringSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqConnect> cRequestManager::getReqConnect( )
{
    if ( mReqConnect.empty( ) )
    {
        auto it = mReqConnectSequenceIds.begin( );
		while ( it != mReqConnectSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqConnectSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqConnect.top( );
        mReqConnect.pop( );
        return top;
    }
}
void cRequestManager::ungetReqConnect( Packet::Request::cReqConnect&& data )
{
    mReqConnect.push( std::move( data ) );
}
bool cRequestManager::isNewReqConnect( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqConnectSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqGetJemSeed> cRequestManager::getReqGetJemSeed( )
{
    if ( mReqGetJemSeed.empty( ) )
    {
        auto it = mReqGetJemSeedSequenceIds.begin( );
		while ( it != mReqGetJemSeedSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqGetJemSeedSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqGetJemSeed.top( );
        mReqGetJemSeed.pop( );
        return top;
    }
}
void cRequestManager::ungetReqGetJemSeed( Packet::Request::cReqGetJemSeed&& data )
{
    mReqGetJemSeed.push( std::move( data ) );
}
bool cRequestManager::isNewReqGetJemSeed( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqGetJemSeedSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqGetJemPoint> cRequestManager::getReqGetJemPoint( )
{
    if ( mReqGetJemPoint.empty( ) )
    {
        auto it = mReqGetJemPointSequenceIds.begin( );
		while ( it != mReqGetJemPointSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqGetJemPointSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqGetJemPoint.top( );
        mReqGetJemPoint.pop( );
        return top;
    }
}
void cRequestManager::ungetReqGetJemPoint( Packet::Request::cReqGetJemPoint&& data )
{
    mReqGetJemPoint.push( std::move( data ) );
}
bool cRequestManager::isNewReqGetJemPoint( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqGetJemPointSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqCheckGetJemPlayer> cRequestManager::getReqCheckGetJemPlayer( )
{
    if ( mReqCheckGetJemPlayer.empty( ) )
    {
        auto it = mReqCheckGetJemPlayerSequenceIds.begin( );
		while ( it != mReqCheckGetJemPlayerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqCheckGetJemPlayerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqCheckGetJemPlayer.top( );
        mReqCheckGetJemPlayer.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCheckGetJemPlayer( Packet::Request::cReqCheckGetJemPlayer&& data )
{
    mReqCheckGetJemPlayer.push( std::move( data ) );
}
bool cRequestManager::isNewReqCheckGetJemPlayer( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqCheckGetJemPlayerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqCheckGetJemQuarry> cRequestManager::getReqCheckGetJemQuarry( )
{
    if ( mReqCheckGetJemQuarry.empty( ) )
    {
        auto it = mReqCheckGetJemQuarrySequenceIds.begin( );
		while ( it != mReqCheckGetJemQuarrySequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqCheckGetJemQuarrySequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqCheckGetJemQuarry.top( );
        mReqCheckGetJemQuarry.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCheckGetJemQuarry( Packet::Request::cReqCheckGetJemQuarry&& data )
{
    mReqCheckGetJemQuarry.push( std::move( data ) );
}
bool cRequestManager::isNewReqCheckGetJemQuarry( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqCheckGetJemQuarrySequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqCheckPlayerRobJem> cRequestManager::getReqCheckPlayerRobJem( )
{
    if ( mReqCheckPlayerRobJem.empty( ) )
    {
        auto it = mReqCheckPlayerRobJemSequenceIds.begin( );
		while ( it != mReqCheckPlayerRobJemSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqCheckPlayerRobJemSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqCheckPlayerRobJem.top( );
        mReqCheckPlayerRobJem.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCheckPlayerRobJem( Packet::Request::cReqCheckPlayerRobJem&& data )
{
    mReqCheckPlayerRobJem.push( std::move( data ) );
}
bool cRequestManager::isNewReqCheckPlayerRobJem( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqCheckPlayerRobJemSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqCheckSetQuarry> cRequestManager::getReqCheckSetQuarry( )
{
    if ( mReqCheckSetQuarry.empty( ) )
    {
        auto it = mReqCheckSetQuarrySequenceIds.begin( );
		while ( it != mReqCheckSetQuarrySequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqCheckSetQuarrySequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqCheckSetQuarry.top( );
        mReqCheckSetQuarry.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCheckSetQuarry( Packet::Request::cReqCheckSetQuarry&& data )
{
    mReqCheckSetQuarry.push( std::move( data ) );
}
bool cRequestManager::isNewReqCheckSetQuarry( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqCheckSetQuarrySequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqCheckPlayerDeath> cRequestManager::getReqCheckPlayerDeath( )
{
    if ( mReqCheckPlayerDeath.empty( ) )
    {
        auto it = mReqCheckPlayerDeathSequenceIds.begin( );
		while ( it != mReqCheckPlayerDeathSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqCheckPlayerDeathSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqCheckPlayerDeath.top( );
        mReqCheckPlayerDeath.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCheckPlayerDeath( Packet::Request::cReqCheckPlayerDeath&& data )
{
    mReqCheckPlayerDeath.push( std::move( data ) );
}
bool cRequestManager::isNewReqCheckPlayerDeath( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqCheckPlayerDeathSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqRespawn> cRequestManager::getReqRespawn( )
{
    if ( mReqRespawn.empty( ) )
    {
        auto it = mReqRespawnSequenceIds.begin( );
		while ( it != mReqRespawnSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqRespawnSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqRespawn.top( );
        mReqRespawn.pop( );
        return top;
    }
}
void cRequestManager::ungetReqRespawn( Packet::Request::cReqRespawn&& data )
{
    mReqRespawn.push( std::move( data ) );
}
bool cRequestManager::isNewReqRespawn( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqRespawnSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqCheckLightBomb> cRequestManager::getReqCheckLightBomb( )
{
    if ( mReqCheckLightBomb.empty( ) )
    {
        auto it = mReqCheckLightBombSequenceIds.begin( );
		while ( it != mReqCheckLightBombSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqCheckLightBombSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqCheckLightBomb.top( );
        mReqCheckLightBomb.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCheckLightBomb( Packet::Request::cReqCheckLightBomb&& data )
{
    mReqCheckLightBomb.push( std::move( data ) );
}
bool cRequestManager::isNewReqCheckLightBomb( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqCheckLightBombSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqDamage> cRequestManager::getReqDamage( )
{
    if ( mReqDamage.empty( ) )
    {
        auto it = mReqDamageSequenceIds.begin( );
		while ( it != mReqDamageSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqDamageSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqDamage.top( );
        mReqDamage.pop( );
        return top;
    }
}
void cRequestManager::ungetReqDamage( Packet::Request::cReqDamage&& data )
{
    mReqDamage.push( std::move( data ) );
}
bool cRequestManager::isNewReqDamage( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqDamageSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqMakeRoom> cRequestManager::getReqMakeRoom( )
{
    if ( mReqMakeRoom.empty( ) )
    {
        auto it = mReqMakeRoomSequenceIds.begin( );
		while ( it != mReqMakeRoomSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqMakeRoomSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqMakeRoom.top( );
        mReqMakeRoom.pop( );
        return top;
    }
}
void cRequestManager::ungetReqMakeRoom( Packet::Request::cReqMakeRoom&& data )
{
    mReqMakeRoom.push( std::move( data ) );
}
bool cRequestManager::isNewReqMakeRoom( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqMakeRoomSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqInRoom> cRequestManager::getReqInRoom( )
{
    if ( mReqInRoom.empty( ) )
    {
        auto it = mReqInRoomSequenceIds.begin( );
		while ( it != mReqInRoomSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqInRoomSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqInRoom.top( );
        mReqInRoom.pop( );
        return top;
    }
}
void cRequestManager::ungetReqInRoom( Packet::Request::cReqInRoom&& data )
{
    mReqInRoom.push( std::move( data ) );
}
bool cRequestManager::isNewReqInRoom( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqInRoomSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqWantTeamIn> cRequestManager::getReqWantTeamIn( )
{
    if ( mReqWantTeamIn.empty( ) )
    {
        auto it = mReqWantTeamInSequenceIds.begin( );
		while ( it != mReqWantTeamInSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqWantTeamInSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqWantTeamIn.top( );
        mReqWantTeamIn.pop( );
        return top;
    }
}
void cRequestManager::ungetReqWantTeamIn( Packet::Request::cReqWantTeamIn&& data )
{
    mReqWantTeamIn.push( std::move( data ) );
}
bool cRequestManager::isNewReqWantTeamIn( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqWantTeamInSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqCheckBeginGame> cRequestManager::getReqCheckBeginGame( )
{
    if ( mReqCheckBeginGame.empty( ) )
    {
        auto it = mReqCheckBeginGameSequenceIds.begin( );
		while ( it != mReqCheckBeginGameSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqCheckBeginGameSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqCheckBeginGame.top( );
        mReqCheckBeginGame.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCheckBeginGame( Packet::Request::cReqCheckBeginGame&& data )
{
    mReqCheckBeginGame.push( std::move( data ) );
}
bool cRequestManager::isNewReqCheckBeginGame( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqCheckBeginGameSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqCheckMember> cRequestManager::getReqCheckMember( )
{
    if ( mReqCheckMember.empty( ) )
    {
        auto it = mReqCheckMemberSequenceIds.begin( );
		while ( it != mReqCheckMemberSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqCheckMemberSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqCheckMember.top( );
        mReqCheckMember.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCheckMember( Packet::Request::cReqCheckMember&& data )
{
    mReqCheckMember.push( std::move( data ) );
}
bool cRequestManager::isNewReqCheckMember( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqCheckMemberSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqEndGamemainSetup> cRequestManager::getReqEndGamemainSetup( )
{
    if ( mReqEndGamemainSetup.empty( ) )
    {
        auto it = mReqEndGamemainSetupSequenceIds.begin( );
		while ( it != mReqEndGamemainSetupSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqEndGamemainSetupSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqEndGamemainSetup.top( );
        mReqEndGamemainSetup.pop( );
        return top;
    }
}
void cRequestManager::ungetReqEndGamemainSetup( Packet::Request::cReqEndGamemainSetup&& data )
{
    mReqEndGamemainSetup.push( std::move( data ) );
}
bool cRequestManager::isNewReqEndGamemainSetup( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqEndGamemainSetupSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqEndStartTimer> cRequestManager::getReqEndStartTimer( )
{
    if ( mReqEndStartTimer.empty( ) )
    {
        auto it = mReqEndStartTimerSequenceIds.begin( );
		while ( it != mReqEndStartTimerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqEndStartTimerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqEndStartTimer.top( );
        mReqEndStartTimer.pop( );
        return top;
    }
}
void cRequestManager::ungetReqEndStartTimer( Packet::Request::cReqEndStartTimer&& data )
{
    mReqEndStartTimer.push( std::move( data ) );
}
bool cRequestManager::isNewReqEndStartTimer( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqEndStartTimerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqAddCannonPower> cRequestManager::getReqAddCannonPower( )
{
    if ( mReqAddCannonPower.empty( ) )
    {
        auto it = mReqAddCannonPowerSequenceIds.begin( );
		while ( it != mReqAddCannonPowerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqAddCannonPowerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqAddCannonPower.top( );
        mReqAddCannonPower.pop( );
        return top;
    }
}
void cRequestManager::ungetReqAddCannonPower( Packet::Request::cReqAddCannonPower&& data )
{
    mReqAddCannonPower.push( std::move( data ) );
}
bool cRequestManager::isNewReqAddCannonPower( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqAddCannonPowerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqCannonPower> cRequestManager::getReqCannonPower( )
{
    if ( mReqCannonPower.empty( ) )
    {
        auto it = mReqCannonPowerSequenceIds.begin( );
		while ( it != mReqCannonPowerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqCannonPowerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqCannonPower.top( );
        mReqCannonPower.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCannonPower( Packet::Request::cReqCannonPower&& data )
{
    mReqCannonPower.push( std::move( data ) );
}
bool cRequestManager::isNewReqCannonPower( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqCannonPowerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqResult> cRequestManager::getReqResult( )
{
    if ( mReqResult.empty( ) )
    {
        auto it = mReqResultSequenceIds.begin( );
		while ( it != mReqResultSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqResultSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqResult.top( );
        mReqResult.pop( );
        return top;
    }
}
void cRequestManager::ungetReqResult( Packet::Request::cReqResult&& data )
{
    mReqResult.push( std::move( data ) );
}
bool cRequestManager::isNewReqResult( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqResultSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
// P=====END=====P
}
