#include <Network/cUDPClientManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <cinder/app/App.h>
#include <limits>
#include <Utility/MessageBox.h>
#include <Network/cUDPManager.h>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cMatching.h>
namespace Network
{
cUDPClientManager::cUDPClientManager( )
    : mCloseSecond( std::numeric_limits<float>::max( ) )
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
    return mConnectServerHandle;
}
void cUDPClientManager::connect( cNetworkHandle const & handle )
{
    mCloseSecond = cinder::app::getElapsedSeconds( ) + 5.0F;

    auto packet = new Packet::Request::cReqConnect( );
    cPacketBuffer packetBuffer;
    packet->createPacket( packetBuffer );
    mSocket.write( handle, packetBuffer.transferredBytes, packetBuffer.buffer.data( ) );
    delete packet;
}
void cUDPClientManager::update( )
{
    updateSend( );
    updateRecv( );
}
void cUDPClientManager::updateSend( )
{
    if ( mConnectServerHandle )
    {
        // 送信するものがあればバッファーから取り出して送る。
        auto& buffer = mSendDataBuffer;
        // 余ってたらパケットを送ります。
        if ( !buffer.empty( ) )
        {
            mSocket.write( mConnectServerHandle, buffer.size( ), buffer.data( ) );
            buffer.clear( );
            buffer.shrink_to_fit( );
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
}
void cUDPClientManager::connection( )
{
    while ( auto p = cResponseManager::getInstance( )->getResConnect( ) )
    {
        mConnectServerHandle = p->mNetworkHandle;
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
void cUDPClientManager::sendDataBufferAdd( cPacketBuffer const & packetBuffer )
{
    if ( !isConnected( ) )
    {
        close( );
        Utility::MessageBoxOk( "connectが成立する前に通信をしないでください。", [ ]
        {
            cSceneManager::getInstance( )->change<Scene::Member::cMatching>( );
        } );
    }

    auto& buf = mSendDataBuffer;

    // パケットが大きくなりそうなら先に送ってしまいます。
    if ( 1024 < buf.size( ) )
    {
        mSocket.write( mConnectServerHandle, buf.size( ), buf.data( ) );
        buf.clear( );
        buf.shrink_to_fit( );
    }

    ubyte2 const& byte = packetBuffer.transferredBytes;
    cBuffer const& buffer = packetBuffer.buffer;

    buf.resize( buf.size( ) + byte );
    memcpy( buf.data( ) + buf.size( ) - byte, &buffer, byte );
}
}