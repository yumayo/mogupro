#include <Network/cResponseManager.h>
namespace Network
{
// P=====BEGIN=====P
Packet::Response::cResString cResponseManager::getResString( )
{
    auto top = mResString.top( );
    mResString.pop( );
    return std::move( top );
}
void cResponseManager::ungetResString( Packet::Response::cResString const & data )
{
    mResString.push( data );
}
Packet::Response::cResPlayer cResponseManager::getResPlayer( )
{
    auto top = mResPlayer.top( );
    mResPlayer.pop( );
    return std::move( top );
}
void cResponseManager::ungetResPlayer( Packet::Response::cResPlayer const & data )
{
    mResPlayer.push( data );
}
Packet::Response::cResGetJemSeed cResponseManager::getResGetJemSeed( )
{
    auto top = mResGetJemSeed.top( );
    mResGetJemSeed.pop( );
    return std::move( top );
}
void cResponseManager::ungetResGetJemSeed( Packet::Response::cResGetJemSeed const & data )
{
    mResGetJemSeed.push( data );
}
Packet::Response::cResGetJemPoint cResponseManager::getResGetJemPoint( )
{
    auto top = mResGetJemPoint.top( );
    mResGetJemPoint.pop( );
    return std::move( top );
}
void cResponseManager::ungetResGetJemPoint( Packet::Response::cResGetJemPoint const & data )
{
    mResGetJemPoint.push( data );
}
Packet::Response::cResCheckGetJem cResponseManager::getResCheckGetJem( )
{
    auto top = mResCheckGetJem.top( );
    mResCheckGetJem.pop( );
    return std::move( top );
}
void cResponseManager::ungetResCheckGetJem( Packet::Response::cResCheckGetJem const & data )
{
    mResCheckGetJem.push( data );
}
Packet::Response::cResCheckPlayerRobJem cResponseManager::getResCheckPlayerRobJem( )
{
    auto top = mResCheckPlayerRobJem.top( );
    mResCheckPlayerRobJem.pop( );
    return std::move( top );
}
void cResponseManager::ungetResCheckPlayerRobJem( Packet::Response::cResCheckPlayerRobJem const & data )
{
    mResCheckPlayerRobJem.push( data );
}
Packet::Response::cResCheckBrakeBlocks cResponseManager::getResCheckBrakeBlocks( )
{
    auto top = mResCheckBrakeBlocks.top( );
    mResCheckBrakeBlocks.pop( );
    return std::move( top );
}
void cResponseManager::ungetResCheckBrakeBlocks( Packet::Response::cResCheckBrakeBlocks const & data )
{
    mResCheckBrakeBlocks.push( data );
}
Packet::Response::cResCheckSetQuarry cResponseManager::getResCheckSetQuarry( )
{
    auto top = mResCheckSetQuarry.top( );
    mResCheckSetQuarry.pop( );
    return std::move( top );
}
void cResponseManager::ungetResCheckSetQuarry( Packet::Response::cResCheckSetQuarry const & data )
{
    mResCheckSetQuarry.push( data );
}
Packet::Response::cResCheckPlayerDeath cResponseManager::getResCheckPlayerDeath( )
{
    auto top = mResCheckPlayerDeath.top( );
    mResCheckPlayerDeath.pop( );
    return std::move( top );
}
void cResponseManager::ungetResCheckPlayerDeath( Packet::Response::cResCheckPlayerDeath const & data )
{
    mResCheckPlayerDeath.push( data );
}
// P=====END=====P
}
