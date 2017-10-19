#include <Network/cRequestManager.h>
namespace Network
{
// P=====BEGIN=====P
Packet::Request::cReqString cRequestManager::getReqString( )
{
    auto top = mReqString.top( );
    mReqString.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqString( Packet::Request::cReqString const & data )
{
    mReqString.push( data );
}
Packet::Request::cReqPlayer cRequestManager::getReqPlayer( )
{
    auto top = mReqPlayer.top( );
    mReqPlayer.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqPlayer( Packet::Request::cReqPlayer const & data )
{
    mReqPlayer.push( data );
}
Packet::Request::cReqGetJemSeed cRequestManager::getReqGetJemSeed( )
{
    auto top = mReqGetJemSeed.top( );
    mReqGetJemSeed.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqGetJemSeed( Packet::Request::cReqGetJemSeed const & data )
{
    mReqGetJemSeed.push( data );
}
Packet::Request::cReqGetJemPoint cRequestManager::getReqGetJemPoint( )
{
    auto top = mReqGetJemPoint.top( );
    mReqGetJemPoint.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqGetJemPoint( Packet::Request::cReqGetJemPoint const & data )
{
    mReqGetJemPoint.push( data );
}
Packet::Request::cReqCheckGetJem cRequestManager::getReqCheckGetJem( )
{
    auto top = mReqCheckGetJem.top( );
    mReqCheckGetJem.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqCheckGetJem( Packet::Request::cReqCheckGetJem const & data )
{
    mReqCheckGetJem.push( data );
}
Packet::Request::cReqCheckPlayerRobJem cRequestManager::getReqCheckPlayerRobJem( )
{
    auto top = mReqCheckPlayerRobJem.top( );
    mReqCheckPlayerRobJem.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqCheckPlayerRobJem( Packet::Request::cReqCheckPlayerRobJem const & data )
{
    mReqCheckPlayerRobJem.push( data );
}
Packet::Request::cReqCheckBreakBlocks cRequestManager::getReqCheckBreakBlocks( )
{
    auto top = mReqCheckBreakBlocks.top( );
    mReqCheckBreakBlocks.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqCheckBreakBlocks( Packet::Request::cReqCheckBreakBlocks const & data )
{
    mReqCheckBreakBlocks.push( data );
}
Packet::Request::cReqCheckSetQuarry cRequestManager::getReqCheckSetQuarry( )
{
    auto top = mReqCheckSetQuarry.top( );
    mReqCheckSetQuarry.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqCheckSetQuarry( Packet::Request::cReqCheckSetQuarry const & data )
{
    mReqCheckSetQuarry.push( data );
}
Packet::Request::cReqCheckPlayerDeath cRequestManager::getReqCheckPlayerDeath( )
{
    auto top = mReqCheckPlayerDeath.top( );
    mReqCheckPlayerDeath.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqCheckPlayerDeath( Packet::Request::cReqCheckPlayerDeath const & data )
{
    mReqCheckPlayerDeath.push( data );
}
// P=====END=====P
}
