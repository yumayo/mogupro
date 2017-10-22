#include <Network/cUDPClientManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
#include <Game/cFieldManager.h>
namespace Network
{
cUDPClientManager::cUDPClientManager( )
    : mConnectServerHandle( "127.0.0.1", 25565 )
    , mBreakBlocksPecket( new Packet::Deliver::cDliBreakBlocks( ) )
{
}
void cUDPClientManager::update( )
{
    allPlayersPosition( );
    allQuarrys( );
    allGems( );
    allBreakBlocks( );
}
void cUDPClientManager::allPlayersPosition( )
{
    auto m = Network::cEventManager::getInstance( );
    while ( auto packet = m->getEvePlayers( ) )
    {
        for ( auto& o : packet->mPlayerFormats )
        {
            //Game::cPlayerManager::getInstance( )->setPlayerPosition( 
            //    o.first, 
            //    o.second.mPosition, 
            //    o.second.mRotation
            //);
        }
    }
}
void cUDPClientManager::allQuarrys( )
{
    auto m = Network::cEventManager::getInstance( );
    while ( auto packet = m->getEveSetQuarry( ) )
    {
        Game::cStrategyManager::getInstance( )->CreateDrill(
            packet->mPosition,
            packet->mDrillId,
            static_cast<Game::Strategy::cDrill::DrillType>( packet->mType ),
            packet->mTeamId == 1 // TODO: チームIDと比較する。
        );
    }
}
void cUDPClientManager::allGems( )
{
    auto eve = Network::cEventManager::getInstance( );
    while ( auto packet = eve->getEveGetJemQuarry( ) )
    {
        Game::cStrategyManager::getInstance( )->HitDrillToGem(
            packet->mDrillId,
            packet->mGemId
        );
    }
    while ( auto packet = eve->getEveGetJemPlayer( ) )
    {
        //Game::cPlayerManager::getInstance( )->mineGem(
        //    packet->mPlayerId,
        //    packet->mGemId
        //);
    }
    auto res = Network::cResponseManager::getInstance( );
    while ( auto packet = res->getResCheckGetJemQuarry( ) )
    {
        if ( !packet->mIsSuccessed ) continue;
        Game::cStrategyManager::getInstance( )->HitDrillToGem(
            packet->mDrillId,
            packet->mGemId
        );
    }
    while ( auto packet = res->getResCheckGetJemPlayer( ) )
    {
        if ( !packet->mIsSuccessed ) continue;
        //Game::cPlayerManager::getInstance( )->mineGem(
        //    packet->mPlayerId,
        //    packet->mGemId
        //);
    }
}
void cUDPClientManager::allBreakBlocks( )
{
    auto eve = Network::cEventManager::getInstance( );
    while ( auto packet = eve->getEveBreakBlocks( ) )
    {
        for ( auto& o : packet->mBreakPositions )
        {
            Game::cFieldManager::getInstance( )->blockBreak(
                o
            );
        }
    }
}
void cUDPClientManager::sendBreakBlock( cinder::vec3 const & position )
{
    // ブロック破壊は一旦バッファに詰めておきます。
    mBreakBlocksPecket->mBreakPositions.emplace_back( position );
}
void cUDPClientManager::sendBreakBlocks( std::vector<cinder::vec3> const & positions )
{
    // ブロック破壊は一旦バッファに詰めておきます。
    std::copy( positions.begin( ), positions.end( ), std::back_inserter( mBreakBlocksPecket->mBreakPositions ) );
}
void cUDPClientManager::sendSetQuarry( cinder::vec3 const & position, ubyte1 drillType )
{
    auto packet = new Packet::Request::cReqCheckSetQuarry( );
    packet->mPosition = position;
    packet->mType = drillType;
    cUDPManager::getInstance( )->send( mConnectServerHandle, packet );
}
void cUDPClientManager::sendPlayerPosition( cinder::vec3 const & position, cinder::quat const & rotation )
{
    auto packet = new Packet::Deliver::cDliPlayer( );
    packet->mPosition = position;
    packet->mRotation = rotation;
    cUDPManager::getInstance( )->send( mConnectServerHandle, packet );
}
void cUDPClientManager::sendGetGemPlayer( ubyte2 gemId )
{
    auto packet = new Packet::Request::cReqCheckGetJemPlayer( );
    packet->mGemId = gemId;
    cUDPManager::getInstance( )->send( mConnectServerHandle, packet );
}
void cUDPClientManager::sendGetGemQuarry( ubyte1 drillId, ubyte2 gemId )
{
    auto packet = new Packet::Request::cReqCheckGetJemQuarry( );
    packet->mDrillId = drillId;
    packet->mGemId = gemId;
    cUDPManager::getInstance( )->send( mConnectServerHandle, packet );
}
void cUDPClientManager::sendBreakBlocks( )
{
    if ( !mBreakBlocksPecket->mBreakPositions.empty( ) )
    {
        cUDPManager::getInstance( )->send( mConnectServerHandle, mBreakBlocksPecket );
        mBreakBlocksPecket = new Packet::Deliver::cDliBreakBlocks( );
    }
}
}