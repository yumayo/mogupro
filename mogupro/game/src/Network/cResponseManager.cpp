#include <Network/cResponseManager.h>
namespace Network
{
// P=====BEGIN=====P
boost::optional<Packet::Response::cResString> cResponseManager::getResString( )
{
    if ( mResString.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResString.top( );
        mResString.pop( );
        return top;
    }
}
void cResponseManager::ungetResString( Packet::Response::cResString&& data )
{
    mResString.push( std::move( data ) );
}
boost::optional<Packet::Response::cResGetJemSeed> cResponseManager::getResGetJemSeed( )
{
    if ( mResGetJemSeed.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResGetJemSeed.top( );
        mResGetJemSeed.pop( );
        return top;
    }
}
void cResponseManager::ungetResGetJemSeed( Packet::Response::cResGetJemSeed&& data )
{
    mResGetJemSeed.push( std::move( data ) );
}
boost::optional<Packet::Response::cResGetJemPoint> cResponseManager::getResGetJemPoint( )
{
    if ( mResGetJemPoint.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResGetJemPoint.top( );
        mResGetJemPoint.pop( );
        return top;
    }
}
void cResponseManager::ungetResGetJemPoint( Packet::Response::cResGetJemPoint&& data )
{
    mResGetJemPoint.push( std::move( data ) );
}
boost::optional<Packet::Response::cResCheckGetJemPlayer> cResponseManager::getResCheckGetJemPlayer( )
{
    if ( mResCheckGetJemPlayer.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResCheckGetJemPlayer.top( );
        mResCheckGetJemPlayer.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckGetJemPlayer( Packet::Response::cResCheckGetJemPlayer&& data )
{
    mResCheckGetJemPlayer.push( std::move( data ) );
}
boost::optional<Packet::Response::cResCheckGetJemQuarry> cResponseManager::getResCheckGetJemQuarry( )
{
    if ( mResCheckGetJemQuarry.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResCheckGetJemQuarry.top( );
        mResCheckGetJemQuarry.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckGetJemQuarry( Packet::Response::cResCheckGetJemQuarry&& data )
{
    mResCheckGetJemQuarry.push( std::move( data ) );
}
boost::optional<Packet::Response::cResCheckPlayerRobJem> cResponseManager::getResCheckPlayerRobJem( )
{
    if ( mResCheckPlayerRobJem.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResCheckPlayerRobJem.top( );
        mResCheckPlayerRobJem.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckPlayerRobJem( Packet::Response::cResCheckPlayerRobJem&& data )
{
    mResCheckPlayerRobJem.push( std::move( data ) );
}
boost::optional<Packet::Response::cResCheckSetQuarry> cResponseManager::getResCheckSetQuarry( )
{
    if ( mResCheckSetQuarry.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResCheckSetQuarry.top( );
        mResCheckSetQuarry.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckSetQuarry( Packet::Response::cResCheckSetQuarry&& data )
{
    mResCheckSetQuarry.push( std::move( data ) );
}
boost::optional<Packet::Response::cResCheckPlayerDeath> cResponseManager::getResCheckPlayerDeath( )
{
    if ( mResCheckPlayerDeath.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResCheckPlayerDeath.top( );
        mResCheckPlayerDeath.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckPlayerDeath( Packet::Response::cResCheckPlayerDeath&& data )
{
    mResCheckPlayerDeath.push( std::move( data ) );
}
boost::optional<Packet::Response::cResMakeRoom> cResponseManager::getResMakeRoom( )
{
    if ( mResMakeRoom.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResMakeRoom.top( );
        mResMakeRoom.pop( );
        return top;
    }
}
void cResponseManager::ungetResMakeRoom( Packet::Response::cResMakeRoom&& data )
{
    mResMakeRoom.push( std::move( data ) );
}
boost::optional<Packet::Response::cResInRoom> cResponseManager::getResInRoom( )
{
    if ( mResInRoom.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResInRoom.top( );
        mResInRoom.pop( );
        return top;
    }
}
void cResponseManager::ungetResInRoom( Packet::Response::cResInRoom&& data )
{
    mResInRoom.push( std::move( data ) );
}
boost::optional<Packet::Response::cResWantTeamIn> cResponseManager::getResWantTeamIn( )
{
    if ( mResWantTeamIn.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResWantTeamIn.top( );
        mResWantTeamIn.pop( );
        return top;
    }
}
void cResponseManager::ungetResWantTeamIn( Packet::Response::cResWantTeamIn&& data )
{
    mResWantTeamIn.push( std::move( data ) );
}
boost::optional<Packet::Response::cResCheckBeginGame> cResponseManager::getResCheckBeginGame( )
{
    if ( mResCheckBeginGame.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResCheckBeginGame.top( );
        mResCheckBeginGame.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckBeginGame( Packet::Response::cResCheckBeginGame&& data )
{
    mResCheckBeginGame.push( std::move( data ) );
}
boost::optional<Packet::Response::cResCheckMember> cResponseManager::getResCheckMember( )
{
    if ( mResCheckMember.empty( ) )
    {
        return boost::none;
    }
    else
    {
        auto top = mResCheckMember.top( );
        mResCheckMember.pop( );
        return top;
    }
}
void cResponseManager::ungetResCheckMember( Packet::Response::cResCheckMember&& data )
{
    mResCheckMember.push( std::move( data ) );
}
// P=====END=====P
}
