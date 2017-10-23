#include <Network/cUDPClientManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <Game/cGemManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cStrategyManager.h>
#include <Game/cFieldManager.h>
#include <limits>
#include <cinder/app/App.h>
#include <Utility/MessageBox.h>
#include <Network/cUDPManager.h>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cMatching.h>
namespace Network
{
cUDPClientManager::cUDPClientManager( )
    : mBreakBlocksPecket( new Packet::Deliver::cDliBreakBlocks( ) )
    , mCloseSecond( std::numeric_limits<float>::max( ) )
{
}
void cUDPClientManager::close( )
{
    mSocket.close( );
}
void cUDPClientManager::open( )
{
    mSocket.open( );
}
bool cUDPClientManager::isConnected( )
{
    return mIsConnected;
}
void cUDPClientManager::connect( cNetworkHandle const & handle )
{
    mConnectServerHandle = handle;
    mCloseSecond = cinder::app::getElapsedSeconds( ) + 5.0F;

    auto packet = new Packet::Request::cReqConnect( );
    cPacketBuffer packetBuffer;
    packet->createPacket( packetBuffer );
    mSocket.write( mConnectServerHandle, packetBuffer.transferredBytes, packetBuffer.buffer.data( ) );
    delete packet;
}
void cUDPClientManager::update( )
{
    updateSend( );
    updateRecv( );
}
void cUDPClientManager::updateSend( )
{
    if ( isConnected( ) )
    {
        // 送信するものがあればバッファーから取り出して送る。
        for ( auto& buffer : mSendDataBuffer )
        {
            // 余ってたらパケットを送ります。
            if ( !buffer.second.empty( ) )
            {
                mSocket.write( buffer.first, buffer.second.size( ), buffer.second.data( ) );
                buffer.second.clear( );
                buffer.second.shrink_to_fit( );
            }
        }
    }
}
void cUDPClientManager::updateRecv( )
{
    // 受信したものがあればバッファーから取り出してパケットの分別を行う。
    while ( !mSocket.emptyChunk( ) )
    {
        auto chunk = mSocket.popChunk( );
        cUDPManager::getInstance( )->onReceive( chunk );
    }

    connection( );
    ping( );
    allPlayersFormat( );
    allQuarrys( );
    allGems( );
    allBreakBlocks( );
}
void cUDPClientManager::connection( )
{
    while ( auto p = cResponseManager::getInstance( )->getResConnect( ) )
    {
        mIsConnected = true;
    }
}
void cUDPClientManager::ping( )
{
    while ( auto p = cEventManager::getInstance( )->getEvePing( ) )
    {
        mCloseSecond = cinder::app::getElapsedSeconds( ) + 5.0F;
    }
    if ( mCloseSecond < cinder::app::getElapsedSeconds( ) )
    {
        close( );
        Utility::MessageBoxOk( "サーバーとの接続が切れました。", [ ]
        {
            cSceneManager::getInstance( )->change<Scene::Member::cMatching>( );
        } );
    }
}
void cUDPClientManager::sendDataBufferAdd( cNetworkHandle const & networkHandle, cPacketBuffer const & packetBuffer )
{
    if ( !isConnected( ) )
    {
        Utility::MessageBoxOk( "接続が完了する前にデータを送信しないで下さい。", [ ]
        {
            cSceneManager::getInstance( )->change<Scene::Member::cMatching>( );
        } );
    }

    auto& buf = mSendDataBuffer[networkHandle];

    // パケットが大きくなりそうなら先に送ってしまいます。
    if ( 1024 < buf.size( ) )
    {
        mSocket.write( networkHandle, buf.size( ), buf.data( ) );
        buf.clear( );
        buf.shrink_to_fit( );
    }

    ubyte2 const& byte = packetBuffer.transferredBytes;
    cBuffer const& buffer = packetBuffer.buffer;

    buf.resize( buf.size( ) + byte );
    memcpy( buf.data( ) + buf.size( ) - byte, &buffer, byte );
}
void cUDPClientManager::allPlayersFormat( )
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
    send( mConnectServerHandle, packet );
}
void cUDPClientManager::sendPlayerFormat( cinder::vec3 const & position, cinder::quat const & rotation )
{
    auto packet = new Packet::Deliver::cDliPlayer( );
    packet->mPosition = position;
    packet->mRotation = rotation;
    send( mConnectServerHandle, packet );
}
void cUDPClientManager::sendGetGemPlayer( ubyte2 gemId )
{
    auto packet = new Packet::Request::cReqCheckGetJemPlayer( );
    packet->mGemId = gemId;
    send( mConnectServerHandle, packet );
}
void cUDPClientManager::sendGetGemQuarry( ubyte1 drillId, ubyte2 gemId )
{
    auto packet = new Packet::Request::cReqCheckGetJemQuarry( );
    packet->mDrillId = drillId;
    packet->mGemId = gemId;
    send( mConnectServerHandle, packet );
}
void cUDPClientManager::sendBreakBlocks( )
{
    if ( !mBreakBlocksPecket->mBreakPositions.empty( ) )
    {
        send( mConnectServerHandle, mBreakBlocksPecket );
        mBreakBlocksPecket = new Packet::Deliver::cDliBreakBlocks( );
    }
}
}