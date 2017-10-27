#include <Game/cServerAdapter.h>
#include <Network/cUDPServerManager.h>
#include <Network/cUDPManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
namespace Game
{
cServerAdapter::cServerAdapter( )
{
    mQuarryId = 0;
    mPlayersPosition[0] = cinder::vec3( 30, 10, 20 );
    mPlayersPosition[1] = cinder::vec3( 32, 10, 20 );
    mPlayersPosition[2] = cinder::vec3( 34, 10, 20 );
    mPlayersPosition[3] = cinder::vec3( 36, 10, 20 );
    mPlayersPosition[4] = cinder::vec3( 30, 10, 30 );
    mPlayersPosition[5] = cinder::vec3( 30, 10, 32 );
    mPlayersPosition[6] = cinder::vec3( 30, 10, 34 );
    mPlayersPosition[7] = cinder::vec3( 30, 10, 36 );
}
cServerAdapter::~cServerAdapter( )
{

}
void cServerAdapter::update( )
{
    sendPlayersPosition( );
    sendSetQuarry( );
    sendGetGemPlayer( );
    sendGetGemQuarry( );
    sendBreakBlocks( );
}
void cServerAdapter::sendPlayersPosition( )
{
    auto dli = Network::cDeliverManager::getInstance( );
    while ( auto packet = dli->getDliPlayer( ) )
    {
        try
        {
            auto id = Network::cUDPServerManager::getInstance( )->getPlayerId( packet->mNetworkHandle );
            mPlayersPosition[id] = packet->mPosition;
        }
        catch ( std::runtime_error e )
        {
            continue;
        }
    }
    auto eventPack = new Network::Packet::Event::cEvePlayers( );
    for ( auto& player : mPlayersPosition )
    {
        eventPack->mPlayerFormats.emplace_back( player.second, cinder::quat( ) );
    }
    Network::cUDPServerManager::getInstance( )->broadcast( eventPack );
}
void cServerAdapter::sendSetQuarry( )
{
    auto req = Network::cRequestManager::getInstance( );
    while ( auto packet = req->getReqCheckSetQuarry( ) )
    {
        mQuarryId += 1;
        mQuarrys.insert( mQuarryId );
        auto quarryPack = new Network::Packet::Response::cResCheckSetQuarry( );
        quarryPack->mDrillId = mQuarryId;
        quarryPack->mIsSucceeded = true;
        quarryPack->mPosition = packet->mPosition;
        quarryPack->mType = packet->mType;
        quarryPack->mTeamId = packet->mTeamId;
        Network::cUDPServerManager::getInstance( )->send( packet->mNetworkHandle, quarryPack );

        auto eventPack = new Network::Packet::Event::cEveSetQuarry( );
        eventPack->mDrillId = quarryPack->mDrillId;
        eventPack->mPosition = quarryPack->mPosition;
        eventPack->mType = quarryPack->mType;
        eventPack->mTeamId = quarryPack->mTeamId;
        Network::cUDPServerManager::getInstance( )->broadcastOthers( packet->mNetworkHandle, eventPack );
    }
}
void cServerAdapter::sendGetGemPlayer( )
{
    auto req = Network::cRequestManager::getInstance( );
    while ( auto packet = req->getReqCheckGetJemPlayer( ) )
    {
        try
        {
            auto resPack = new Network::Packet::Response::cResCheckGetJemPlayer( );
            auto playerId = Network::cUDPServerManager::getInstance( )->getPlayerId( packet->mNetworkHandle );
            auto isSuccess = mGems.insert( packet->mGemId ).second;

            resPack->mPlayerId = playerId;
            resPack->mGemId = packet->mGemId;
            resPack->mIsSuccessed = isSuccess;
            Network::cUDPServerManager::getInstance( )->send( packet->mNetworkHandle, resPack );

            // 成功なら他の人に俺、宝石採ったぜアピールをします。
            if ( isSuccess )
            {
                auto eventPack = new Network::Packet::Event::cEveGetJemPlayer( );
                eventPack->mPlayerId = playerId;
                eventPack->mGemId = packet->mGemId;
                Network::cUDPServerManager::getInstance( )->broadcastOthers( packet->mNetworkHandle, eventPack );
            }
        }
        catch ( std::runtime_error e )
        {
            continue;
        }
    }
}
void cServerAdapter::sendGetGemQuarry( )
{
    auto req = Network::cRequestManager::getInstance( );
    while ( auto packet = req->getReqCheckGetJemQuarry( ) )
    {
        auto resPack = new Network::Packet::Response::cResCheckGetJemQuarry( );
        auto isSuccess = mGems.insert( packet->mGemId ).second;

        resPack->mDrillId = packet->mDrillId;
        resPack->mGemId = packet->mGemId;
        resPack->mIsSuccessed = isSuccess;
        Network::cUDPServerManager::getInstance( )->send( packet->mNetworkHandle, resPack );

        // 成功なら他の人に俺の掘削機、宝石採ったぜアピールをします。
        if ( isSuccess )
        {
            auto eventPack = new Network::Packet::Event::cEveGetJemQuarry( );
            eventPack->mDrillId = packet->mDrillId;
            eventPack->mGemId = packet->mGemId;
            Network::cUDPServerManager::getInstance( )->broadcastOthers( packet->mNetworkHandle, eventPack );
        }
    }
}
void cServerAdapter::sendBreakBlocks( )
{
    auto dli = ::Network::cDeliverManager::getInstance( );
    auto breakBlocksPacket = new Network::Packet::Event::cEveBreakBlocks( );
    while ( auto packet = dli->getDliBreakBlocks( ) )
    {
        std::copy( packet->mBreakPositions.begin( ),
                   packet->mBreakPositions.end( ),
                   std::back_inserter( breakBlocksPacket->mBreakPositions ) );
    }
    Network::cUDPServerManager::getInstance( )->broadcast( breakBlocksPacket );
}
}
