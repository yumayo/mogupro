#include <Network/cEventManager.h>
#include <cinder/app/App.h>
namespace Network
{
// P=====BEGIN=====P
boost::optional<Packet::Event::cEveString> cEventManager::getEveString( )
{
    if ( mEveString.empty( ) )
    {
        auto it = mEveStringSequenceIds.begin( );
		while ( it != mEveStringSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveStringSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveString.top( );
        mEveString.pop( );
        return top;
    }
}
void cEventManager::ungetEveString( Packet::Event::cEveString&& data )
{
    mEveString.push( std::move( data ) );
}
bool cEventManager::isNewEveString( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveStringSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEvePing> cEventManager::getEvePing( )
{
    if ( mEvePing.empty( ) )
    {
        auto it = mEvePingSequenceIds.begin( );
		while ( it != mEvePingSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEvePingSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEvePing.top( );
        mEvePing.pop( );
        return top;
    }
}
void cEventManager::ungetEvePing( Packet::Event::cEvePing&& data )
{
    mEvePing.push( std::move( data ) );
}
bool cEventManager::isNewEvePing( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEvePingSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEvePlayers> cEventManager::getEvePlayers( )
{
    if ( mEvePlayers.empty( ) )
    {
        auto it = mEvePlayersSequenceIds.begin( );
		while ( it != mEvePlayersSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEvePlayersSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEvePlayers.top( );
        mEvePlayers.pop( );
        return top;
    }
}
void cEventManager::ungetEvePlayers( Packet::Event::cEvePlayers&& data )
{
    mEvePlayers.push( std::move( data ) );
}
bool cEventManager::isNewEvePlayers( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEvePlayersSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveGetJemPlayer> cEventManager::getEveGetJemPlayer( )
{
    if ( mEveGetJemPlayer.empty( ) )
    {
        auto it = mEveGetJemPlayerSequenceIds.begin( );
		while ( it != mEveGetJemPlayerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveGetJemPlayerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveGetJemPlayer.top( );
        mEveGetJemPlayer.pop( );
        return top;
    }
}
void cEventManager::ungetEveGetJemPlayer( Packet::Event::cEveGetJemPlayer&& data )
{
    mEveGetJemPlayer.push( std::move( data ) );
}
bool cEventManager::isNewEveGetJemPlayer( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveGetJemPlayerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveGetJemQuarry> cEventManager::getEveGetJemQuarry( )
{
    if ( mEveGetJemQuarry.empty( ) )
    {
        auto it = mEveGetJemQuarrySequenceIds.begin( );
		while ( it != mEveGetJemQuarrySequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveGetJemQuarrySequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveGetJemQuarry.top( );
        mEveGetJemQuarry.pop( );
        return top;
    }
}
void cEventManager::ungetEveGetJemQuarry( Packet::Event::cEveGetJemQuarry&& data )
{
    mEveGetJemQuarry.push( std::move( data ) );
}
bool cEventManager::isNewEveGetJemQuarry( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveGetJemQuarrySequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveBreakBlocks> cEventManager::getEveBreakBlocks( )
{
    if ( mEveBreakBlocks.empty( ) )
    {
        auto it = mEveBreakBlocksSequenceIds.begin( );
		while ( it != mEveBreakBlocksSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveBreakBlocksSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveBreakBlocks.top( );
        mEveBreakBlocks.pop( );
        return top;
    }
}
void cEventManager::ungetEveBreakBlocks( Packet::Event::cEveBreakBlocks&& data )
{
    mEveBreakBlocks.push( std::move( data ) );
}
bool cEventManager::isNewEveBreakBlocks( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveBreakBlocksSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveSetQuarry> cEventManager::getEveSetQuarry( )
{
    if ( mEveSetQuarry.empty( ) )
    {
        auto it = mEveSetQuarrySequenceIds.begin( );
		while ( it != mEveSetQuarrySequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveSetQuarrySequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveSetQuarry.top( );
        mEveSetQuarry.pop( );
        return top;
    }
}
void cEventManager::ungetEveSetQuarry( Packet::Event::cEveSetQuarry&& data )
{
    mEveSetQuarry.push( std::move( data ) );
}
bool cEventManager::isNewEveSetQuarry( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveSetQuarrySequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEvePlayerRobJem> cEventManager::getEvePlayerRobJem( )
{
    if ( mEvePlayerRobJem.empty( ) )
    {
        auto it = mEvePlayerRobJemSequenceIds.begin( );
		while ( it != mEvePlayerRobJemSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEvePlayerRobJemSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEvePlayerRobJem.top( );
        mEvePlayerRobJem.pop( );
        return top;
    }
}
void cEventManager::ungetEvePlayerRobJem( Packet::Event::cEvePlayerRobJem&& data )
{
    mEvePlayerRobJem.push( std::move( data ) );
}
bool cEventManager::isNewEvePlayerRobJem( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEvePlayerRobJemSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEvePlayerDeath> cEventManager::getEvePlayerDeath( )
{
    if ( mEvePlayerDeath.empty( ) )
    {
        auto it = mEvePlayerDeathSequenceIds.begin( );
		while ( it != mEvePlayerDeathSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEvePlayerDeathSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEvePlayerDeath.top( );
        mEvePlayerDeath.pop( );
        return top;
    }
}
void cEventManager::ungetEvePlayerDeath( Packet::Event::cEvePlayerDeath&& data )
{
    mEvePlayerDeath.push( std::move( data ) );
}
bool cEventManager::isNewEvePlayerDeath( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEvePlayerDeathSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveRespawn> cEventManager::getEveRespawn( )
{
    if ( mEveRespawn.empty( ) )
    {
        auto it = mEveRespawnSequenceIds.begin( );
		while ( it != mEveRespawnSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveRespawnSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveRespawn.top( );
        mEveRespawn.pop( );
        return top;
    }
}
void cEventManager::ungetEveRespawn( Packet::Event::cEveRespawn&& data )
{
    mEveRespawn.push( std::move( data ) );
}
bool cEventManager::isNewEveRespawn( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveRespawnSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveAddCannonPower> cEventManager::getEveAddCannonPower( )
{
    if ( mEveAddCannonPower.empty( ) )
    {
        auto it = mEveAddCannonPowerSequenceIds.begin( );
		while ( it != mEveAddCannonPowerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveAddCannonPowerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveAddCannonPower.top( );
        mEveAddCannonPower.pop( );
        return top;
    }
}
void cEventManager::ungetEveAddCannonPower( Packet::Event::cEveAddCannonPower&& data )
{
    mEveAddCannonPower.push( std::move( data ) );
}
bool cEventManager::isNewEveAddCannonPower( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveAddCannonPowerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveLightBomb> cEventManager::getEveLightBomb( )
{
    if ( mEveLightBomb.empty( ) )
    {
        auto it = mEveLightBombSequenceIds.begin( );
		while ( it != mEveLightBombSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveLightBombSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveLightBomb.top( );
        mEveLightBomb.pop( );
        return top;
    }
}
void cEventManager::ungetEveLightBomb( Packet::Event::cEveLightBomb&& data )
{
    mEveLightBomb.push( std::move( data ) );
}
bool cEventManager::isNewEveLightBomb( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveLightBombSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveTeamMember> cEventManager::getEveTeamMember( )
{
    if ( mEveTeamMember.empty( ) )
    {
        auto it = mEveTeamMemberSequenceIds.begin( );
		while ( it != mEveTeamMemberSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveTeamMemberSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveTeamMember.top( );
        mEveTeamMember.pop( );
        return top;
    }
}
void cEventManager::ungetEveTeamMember( Packet::Event::cEveTeamMember&& data )
{
    mEveTeamMember.push( std::move( data ) );
}
bool cEventManager::isNewEveTeamMember( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveTeamMemberSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveDamage> cEventManager::getEveDamage( )
{
    if ( mEveDamage.empty( ) )
    {
        auto it = mEveDamageSequenceIds.begin( );
		while ( it != mEveDamageSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveDamageSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveDamage.top( );
        mEveDamage.pop( );
        return top;
    }
}
void cEventManager::ungetEveDamage( Packet::Event::cEveDamage&& data )
{
    mEveDamage.push( std::move( data ) );
}
bool cEventManager::isNewEveDamage( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveDamageSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEvePlayerAttack> cEventManager::getEvePlayerAttack( )
{
    if ( mEvePlayerAttack.empty( ) )
    {
        auto it = mEvePlayerAttackSequenceIds.begin( );
		while ( it != mEvePlayerAttackSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEvePlayerAttackSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEvePlayerAttack.top( );
        mEvePlayerAttack.pop( );
        return top;
    }
}
void cEventManager::ungetEvePlayerAttack( Packet::Event::cEvePlayerAttack&& data )
{
    mEvePlayerAttack.push( std::move( data ) );
}
bool cEventManager::isNewEvePlayerAttack( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEvePlayerAttackSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Event::cEveFragmentGem> cEventManager::getEveFragmentGem( )
{
    if ( mEveFragmentGem.empty( ) )
    {
        auto it = mEveFragmentGemSequenceIds.begin( );
		while ( it != mEveFragmentGemSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mEveFragmentGemSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mEveFragmentGem.top( );
        mEveFragmentGem.pop( );
        return top;
    }
}
void cEventManager::ungetEveFragmentGem( Packet::Event::cEveFragmentGem&& data )
{
    mEveFragmentGem.push( std::move( data ) );
}
bool cEventManager::isNewEveFragmentGem( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mEveFragmentGemSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
// P=====END=====P
}
