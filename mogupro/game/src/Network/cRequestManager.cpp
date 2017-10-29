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
boost::optional<Packet::Request::cReqConnect> cRequestManager::getReqConnect( )
{
    if ( mReqConnect.empty( ) )
    {
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
boost::optional<Packet::Request::cReqCheckGetJemPlayer> cRequestManager::getReqCheckGetJemPlayer( )
{
    if ( mReqCheckGetJemPlayer.empty( ) )
    {
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
boost::optional<Packet::Request::cReqCheckGetJemQuarry> cRequestManager::getReqCheckGetJemQuarry( )
{
    if ( mReqCheckGetJemQuarry.empty( ) )
    {
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
boost::optional<Packet::Request::cReqEndGamemainSetup> cRequestManager::getReqEndGamemainSetup( )
{
    if ( mReqEndGamemainSetup.empty( ) )
    {
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
// P=====END=====P
}
