#include <Network/cEventManager.h>
namespace Network
{
// P=====BEGIN=====P
boost::optional<Packet::Event::cEveString> cEventManager::getEveString( )
{
    if ( mEveString.empty( ) )
    {
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
boost::optional<Packet::Event::cEvePlayers> cEventManager::getEvePlayers( )
{
    if ( mEvePlayers.empty( ) )
    {
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
boost::optional<Packet::Event::cEveGetJemPlayer> cEventManager::getEveGetJemPlayer( )
{
    if ( mEveGetJemPlayer.empty( ) )
    {
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
boost::optional<Packet::Event::cEveGetJemQuarry> cEventManager::getEveGetJemQuarry( )
{
    if ( mEveGetJemQuarry.empty( ) )
    {
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
boost::optional<Packet::Event::cEveBreakBlocks> cEventManager::getEveBreakBlocks( )
{
    if ( mEveBreakBlocks.empty( ) )
    {
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
boost::optional<Packet::Event::cEveSetQuarry> cEventManager::getEveSetQuarry( )
{
    if ( mEveSetQuarry.empty( ) )
    {
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
boost::optional<Packet::Event::cEvePlayerRobJem> cEventManager::getEvePlayerRobJem( )
{
    if ( mEvePlayerRobJem.empty( ) )
    {
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
boost::optional<Packet::Event::cEvePlayerDeath> cEventManager::getEvePlayerDeath( )
{
    if ( mEvePlayerDeath.empty( ) )
    {
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
// P=====END=====P
}
