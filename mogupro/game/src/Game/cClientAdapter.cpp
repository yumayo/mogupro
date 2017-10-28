#include <Game/cClientAdapter.h>
#include <Network/cUDPClientManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
#include <Game/cFieldManager.h>
namespace Game
{
cClientAdapter::cClientAdapter( )
    : mBreakBlocksPecket( new Network::Packet::Deliver::cDliBreakBlocks( ) )
{
}
cClientAdapter::~cClientAdapter( )
{
    delete mBreakBlocksPecket;
}
void cClientAdapter::update( )
{
    // ブロックだけは個々のタイミングで送信します。
    sendBreakBlocks( );

    // サーバーから受信したものがあった場合は取り出して、
    // 各マネージャーに伝えます。
    recvAllPlayers( );
    recvAllQuarrys( );
    recvAllGems( );
    recvAllBreakBlocks( );
}
void cClientAdapter::recvAllPlayers( )
{
    auto m = ::Network::cEventManager::getInstance( );
    while ( auto packet = m->getEvePlayers( ) )
    {
        auto players = Game::cPlayerManager::getInstance( )->getPlayers( );
        for ( auto& o : packet->mPlayerFormats )
        {
            players[o.playerId]->setPos( o.position );
        }
    }
}
void cClientAdapter::recvAllQuarrys( )
{
    auto res = Network::cResponseManager::getInstance( );
    while ( auto packet = res->getResCheckSetQuarry( ) )
    {
        if ( packet->mIsSucceeded )
        {
            Game::cStrategyManager::getInstance( )->CreateDrill(
                packet->mPosition,
                packet->mDrillId,
                static_cast<Game::Strategy::cDrill::DrillType>( packet->mType ),
                true
            );
        }
    }
    auto eve = Network::cEventManager::getInstance( );
    while ( auto packet = eve->getEveSetQuarry( ) )
    {
        Game::cStrategyManager::getInstance( )->CreateDrill(
            packet->mPosition,
            packet->mDrillId,
            static_cast<Game::Strategy::cDrill::DrillType>( packet->mType ),
            packet->mTeamId == Game::cPlayerManager::getInstance()->getActivePlayerTeamId( )
        );
    }
}
void cClientAdapter::recvAllGems( )
{
    auto eve = ::Network::cEventManager::getInstance( );
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
    auto res = ::Network::cResponseManager::getInstance( );
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
void cClientAdapter::recvAllBreakBlocks( )
{
    auto eve = ::Network::cEventManager::getInstance( );
    while ( auto packet = eve->getEveBreakBlocks( ) )
    {
        for ( auto& o : packet->mBreakFormats )
        {
            Game::cFieldManager::getInstance( )->blockBreakNetwork(
                o.position, o.radius
            );
        }
    }
}
void cClientAdapter::sendBreakBlock( cinder::vec3 const & position, float radius )
{
    // ブロック破壊は一旦バッファに詰めておきます。
    mBreakBlocksPecket->mBreakFormats.emplace_back( position, radius );
}
void cClientAdapter::sendSetQuarry( cinder::vec3 const & position, Network::ubyte1 drillType )
{
    auto packet = new Network::Packet::Request::cReqCheckSetQuarry( );
    packet->mPosition = position;
    packet->mType = drillType;
    packet->mTeamId = cPlayerManager::getInstance( )->getActivePlayerTeamId( );
    Network::cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendPlayer( cinder::vec3 const & position, cinder::quat const & rotation )
{
    auto packet = new Network::Packet::Deliver::cDliPlayer( );
    packet->mPosition = position;
    packet->mRotation = rotation;
    Network::cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendGetGemPlayer( Network::ubyte2 gemId )
{
    auto packet = new Network::Packet::Request::cReqCheckGetJemPlayer( );
    packet->mGemId = gemId;
    Network::cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendGetGemQuarry( Network::ubyte2 drillId, Network::ubyte2 gemId )
{
    auto packet = new Network::Packet::Request::cReqCheckGetJemQuarry( );
    packet->mDrillId = drillId;
    packet->mGemId = gemId;
    Network::cUDPClientManager::getInstance( )->send( packet );
}
void cClientAdapter::sendBreakBlocks( )
{
    if ( !mBreakBlocksPecket->mBreakFormats.empty( ) )
    {
        Network::cUDPClientManager::getInstance( )->send( mBreakBlocksPecket );
        mBreakBlocksPecket = new Network::Packet::Deliver::cDliBreakBlocks( );
    }
}
}
