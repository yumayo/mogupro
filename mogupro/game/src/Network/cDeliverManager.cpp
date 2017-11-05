#include <Network/cDeliverManager.h>
#include <cinder/app/App.h>
namespace Network
{
// P=====BEGIN=====P
boost::optional<Packet::Deliver::cDliString> cDeliverManager::getDliString( )
{
    if ( mDliString.empty( ) )
    {
        auto it = mDliStringSequenceIds.begin( );
		while ( it != mDliStringSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mDliStringSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mDliString.top( );
        mDliString.pop( );
        return top;
    }
}
void cDeliverManager::ungetDliString( Packet::Deliver::cDliString&& data )
{
    mDliString.push( std::move( data ) );
}
bool cDeliverManager::isNewDliString( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mDliStringSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Deliver::cDliPing> cDeliverManager::getDliPing( )
{
    if ( mDliPing.empty( ) )
    {
        auto it = mDliPingSequenceIds.begin( );
		while ( it != mDliPingSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mDliPingSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mDliPing.top( );
        mDliPing.pop( );
        return top;
    }
}
void cDeliverManager::ungetDliPing( Packet::Deliver::cDliPing&& data )
{
    mDliPing.push( std::move( data ) );
}
bool cDeliverManager::isNewDliPing( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mDliPingSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Deliver::cDliPlayer> cDeliverManager::getDliPlayer( )
{
    if ( mDliPlayer.empty( ) )
    {
        auto it = mDliPlayerSequenceIds.begin( );
		while ( it != mDliPlayerSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mDliPlayerSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mDliPlayer.top( );
        mDliPlayer.pop( );
        return top;
    }
}
void cDeliverManager::ungetDliPlayer( Packet::Deliver::cDliPlayer&& data )
{
    mDliPlayer.push( std::move( data ) );
}
bool cDeliverManager::isNewDliPlayer( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mDliPlayerSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
boost::optional<Packet::Deliver::cDliBreakBlocks> cDeliverManager::getDliBreakBlocks( )
{
    if ( mDliBreakBlocks.empty( ) )
    {
        auto it = mDliBreakBlocksSequenceIds.begin( );
		while ( it != mDliBreakBlocksSequenceIds.end( ) ) 
		{
			if ( it->second < cinder::app::getElapsedSeconds( ) - RELIABLE_HOLD_SECOND )
			{
				mDliBreakBlocksSequenceIds.erase( it++ );
			}
			else ++it;
		}
        return boost::none;
    }
    else
    {
        auto top = mDliBreakBlocks.top( );
        mDliBreakBlocks.pop( );
        return top;
    }
}
void cDeliverManager::ungetDliBreakBlocks( Packet::Deliver::cDliBreakBlocks&& data )
{
    mDliBreakBlocks.push( std::move( data ) );
}
bool cDeliverManager::isNewDliBreakBlocks( Packet::PacketHeader const& header )
{
	if ( ( header.mState & Packet::PacketHeader::RELIABLE ) != Packet::PacketHeader::RELIABLE ) return true;
    auto status = mDliBreakBlocksSequenceIds.insert( std::make_pair( header.mSequenceId, cinder::app::getElapsedSeconds( ) ) );
    status.first->second = cinder::app::getElapsedSeconds( );
	return status.second;
}
// P=====END=====P
}
