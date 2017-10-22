#include <Network/cDeliverManager.h>
namespace Network
{
// P=====BEGIN=====P
boost::optional<Packet::Deliver::cDliString> cDeliverManager::getDliString( )
{
    if ( mDliString.empty( ) )
    {
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
boost::optional<Packet::Deliver::cDliPlayer> cDeliverManager::getDliPlayer( )
{
    if ( mDliPlayer.empty( ) )
    {
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
boost::optional<Packet::Deliver::cDliBreakBlocks> cDeliverManager::getDliBreakBlocks( )
{
    if ( mDliBreakBlocks.empty( ) )
    {
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
// P=====END=====P
}
