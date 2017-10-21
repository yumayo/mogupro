#include <Network/cEventManager.h>
namespace Network
{
// P=====BEGIN=====P
Packet::Event::cEveString cEventManager::getEveString( )
{
    auto top = mEveString.top( );
    mEveString.pop( );
    return std::move( top );
}
void cEventManager::ungetEveString( Packet::Event::cEveString const & data )
{
    mEveString.push( data );
}
Packet::Event::cEveGetJem cEventManager::getEveGetJem( )
{
    auto top = mEveGetJem.top( );
    mEveGetJem.pop( );
    return std::move( top );
}
void cEventManager::ungetEveGetJem( Packet::Event::cEveGetJem const & data )
{
    mEveGetJem.push( data );
}
Packet::Event::cEvePlayerRobJem cEventManager::getEvePlayerRobJem( )
{
    auto top = mEvePlayerRobJem.top( );
    mEvePlayerRobJem.pop( );
    return std::move( top );
}
void cEventManager::ungetEvePlayerRobJem( Packet::Event::cEvePlayerRobJem const & data )
{
    mEvePlayerRobJem.push( data );
}
Packet::Event::cEvePlayerDeath cEventManager::getEvePlayerDeath( )
{
    auto top = mEvePlayerDeath.top( );
    mEvePlayerDeath.pop( );
    return std::move( top );
}
void cEventManager::ungetEvePlayerDeath( Packet::Event::cEvePlayerDeath const & data )
{
    mEvePlayerDeath.push( data );
}
// P=====END=====P
}
