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
boost::optional<Packet::Request::cReqGetJemPlayer> cRequestManager::getReqGetJemPlayer( )
{
    if ( mReqGetJemPlayer.empty( ) )
    {
        auto it = mReqGetJemPlayerSequenceIds.begin( );
		while ( it != mReqGetJemPlayerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqGetJemPlayerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqGetJemPlayer.top( );
        mReqGetJemPlayer.pop( );
        return top;
    }
}
void cRequestManager::ungetReqGetJemPlayer( Packet::Request::cReqGetJemPlayer&& data )
{
    mReqGetJemPlayer.push( std::move( data ) );
}
bool cRequestManager::isNewReqGetJemPlayer( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqGetJemPlayerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqGetJemQuarry> cRequestManager::getReqGetJemQuarry( )
{
    if ( mReqGetJemQuarry.empty( ) )
    {
        auto it = mReqGetJemQuarrySequenceIds.begin( );
		while ( it != mReqGetJemQuarrySequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqGetJemQuarrySequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqGetJemQuarry.top( );
        mReqGetJemQuarry.pop( );
        return top;
    }
}
void cRequestManager::ungetReqGetJemQuarry( Packet::Request::cReqGetJemQuarry&& data )
{
    mReqGetJemQuarry.push( std::move( data ) );
}
bool cRequestManager::isNewReqGetJemQuarry( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqGetJemQuarrySequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqPlayerRobJem> cRequestManager::getReqPlayerRobJem( )
{
    if ( mReqPlayerRobJem.empty( ) )
    {
        auto it = mReqPlayerRobJemSequenceIds.begin( );
		while ( it != mReqPlayerRobJemSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqPlayerRobJemSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqPlayerRobJem.top( );
        mReqPlayerRobJem.pop( );
        return top;
    }
}
void cRequestManager::ungetReqPlayerRobJem( Packet::Request::cReqPlayerRobJem&& data )
{
    mReqPlayerRobJem.push( std::move( data ) );
}
bool cRequestManager::isNewReqPlayerRobJem( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqPlayerRobJemSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqSetQuarry> cRequestManager::getReqSetQuarry( )
{
    if ( mReqSetQuarry.empty( ) )
    {
        auto it = mReqSetQuarrySequenceIds.begin( );
		while ( it != mReqSetQuarrySequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqSetQuarrySequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqSetQuarry.top( );
        mReqSetQuarry.pop( );
        return top;
    }
}
void cRequestManager::ungetReqSetQuarry( Packet::Request::cReqSetQuarry&& data )
{
    mReqSetQuarry.push( std::move( data ) );
}
bool cRequestManager::isNewReqSetQuarry( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqSetQuarrySequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Request::cReqPlayerDeath> cRequestManager::getReqPlayerDeath( )
{
    if ( mReqPlayerDeath.empty( ) )
    {
        auto it = mReqPlayerDeathSequenceIds.begin( );
		while ( it != mReqPlayerDeathSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqPlayerDeathSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqPlayerDeath.top( );
        mReqPlayerDeath.pop( );
        return top;
    }
}
void cRequestManager::ungetReqPlayerDeath( Packet::Request::cReqPlayerDeath&& data )
{
    mReqPlayerDeath.push( std::move( data ) );
}
bool cRequestManager::isNewReqPlayerDeath( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqPlayerDeathSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
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
boost::optional<Packet::Request::cReqLightBomb> cRequestManager::getReqLightBomb( )
{
    if ( mReqLightBomb.empty( ) )
    {
        auto it = mReqLightBombSequenceIds.begin( );
		while ( it != mReqLightBombSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqLightBombSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqLightBomb.top( );
        mReqLightBomb.pop( );
        return top;
    }
}
void cRequestManager::ungetReqLightBomb( Packet::Request::cReqLightBomb&& data )
{
    mReqLightBomb.push( std::move( data ) );
}
bool cRequestManager::isNewReqLightBomb( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqLightBombSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
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
boost::optional<Packet::Request::cReqPlayerAttack> cRequestManager::getReqPlayerAttack( )
{
    if ( mReqPlayerAttack.empty( ) )
    {
        auto it = mReqPlayerAttackSequenceIds.begin( );
		while ( it != mReqPlayerAttackSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mReqPlayerAttackSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mReqPlayerAttack.top( );
        mReqPlayerAttack.pop( );
        return top;
    }
}
void cRequestManager::ungetReqPlayerAttack( Packet::Request::cReqPlayerAttack&& data )
{
    mReqPlayerAttack.push( std::move( data ) );
}
bool cRequestManager::isNewReqPlayerAttack( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mReqPlayerAttackSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
// P=====END=====P
}
