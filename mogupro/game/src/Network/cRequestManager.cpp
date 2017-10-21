#include <Network/cRequestManager.h>
namespace Network
{
// P=====BEGIN=====P
boost::optional<Packet::Request::cReqString> cRequestManager::getReqString( )
{
    if ( mReqString.empty( ) )
    {
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
boost::optional<Packet::Request::cReqPlayer> cRequestManager::getReqPlayer( )
{
    if ( mReqPlayer.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mReqPlayer.top( );
        mReqPlayer.pop( );
        return top;
    }
}
void cRequestManager::ungetReqPlayer( Packet::Request::cReqPlayer&& data )
{
    mReqPlayer.push( std::move( data ) );
}
boost::optional<Packet::Request::cReqGetJemSeed> cRequestManager::getReqGetJemSeed( )
{
    if ( mReqGetJemSeed.empty( ) )
    {
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
boost::optional<Packet::Request::cReqGetJemPoint> cRequestManager::getReqGetJemPoint( )
{
    if ( mReqGetJemPoint.empty( ) )
    {
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
boost::optional<Packet::Request::cReqCheckGetJem> cRequestManager::getReqCheckGetJem( )
{
    if ( mReqCheckGetJem.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mReqCheckGetJem.top( );
        mReqCheckGetJem.pop( );
        return top;
    }
}
void cRequestManager::ungetReqCheckGetJem( Packet::Request::cReqCheckGetJem&& data )
{
    mReqCheckGetJem.push( std::move( data ) );
}
boost::optional<Packet::Request::cReqCheckPlayerRobJem> cRequestManager::getReqCheckPlayerRobJem( )
{
    if ( mReqCheckPlayerRobJem.empty( ) )
    {
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
boost::optional<Packet::Request::cReqBreakBlocks> cRequestManager::getReqBreakBlocks( )
{
    if ( mReqBreakBlocks.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mReqBreakBlocks.top( );
        mReqBreakBlocks.pop( );
        return top;
    }
}
void cRequestManager::ungetReqBreakBlocks( Packet::Request::cReqBreakBlocks&& data )
{
    mReqBreakBlocks.push( std::move( data ) );
}
boost::optional<Packet::Request::cReqCheckSetQuarry> cRequestManager::getReqCheckSetQuarry( )
{
    if ( mReqCheckSetQuarry.empty( ) )
    {
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
boost::optional<Packet::Request::cReqCheckPlayerDeath> cRequestManager::getReqCheckPlayerDeath( )
{
    if ( mReqCheckPlayerDeath.empty( ) )
    {
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
boost::optional<Packet::Request::cReqMakeRoom> cRequestManager::getReqMakeRoom( )
{
    if ( mReqMakeRoom.empty( ) )
    {
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
boost::optional<Packet::Request::cReqInRoom> cRequestManager::getReqInRoom( )
{
    if ( mReqInRoom.empty( ) )
    {
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
boost::optional<Packet::Request::cReqWantTeamIn> cRequestManager::getReqWantTeamIn( )
{
    if ( mReqWantTeamIn.empty( ) )
    {
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
boost::optional<Packet::Request::cReqCheckBeginGame> cRequestManager::getReqCheckBeginGame( )
{
    if ( mReqCheckBeginGame.empty( ) )
    {
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
boost::optional<Packet::Request::cReqCheckMember> cRequestManager::getReqCheckMember( )
{
    if ( mReqCheckMember.empty( ) )
    {
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
// P=====END=====P
}
