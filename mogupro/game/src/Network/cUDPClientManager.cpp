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
#include <Scene/Member/cTitle.h>
#include <Node/action.hpp>
#include <Network/IpHost.h>
namespace Network
{
cUDPClientManager::cUDPClientManager( )
    : mCloseSecond( std::numeric_limits<float>::max( ) )
    , mRoot( Node::node::create( ) )
    , mConnectSecond( std::numeric_limits<float>::max( ) )
	, mSequenceId( 0U )
{
    mRoot->set_schedule_update( );
}
void cUDPClientManager::close( )
{
    mSocket.close( );
    mRoot->remove_action_by_name( "ping" );
}
void cUDPClientManager::open( )
{
    mSocket.open( );
}
bool cUDPClientManager::isConnected( )
{
    return mConnectServerHandle;
}
void cUDPClientManager::connect( std::string const& ipAddress )
{
	send( new Packet::Request::cReqConnect( ), true );
	mConnectSecond = cinder::app::getElapsedSeconds( ) + HOLD_SECOND;
}
void cUDPClientManager::connectOfflineServer( )
{
    connect( Network::getLocalIpAddressHost( ) );
}
void cUDPClientManager::update( float delta )
{
    updateRecv( );
    updateSend( );
    mRoot->entry_update( delta );
}
void cUDPClientManager::updateSend( )
{
	auto& handle = mConnectServerHandle;
	auto& buf = mSendDataBuffer;

	// リライアブルなデータを詰めます。
	auto&& reliableData = std::move( mReliableManager.update( ) );
	std::copy( reliableData.begin( ), reliableData.end( ), std::back_inserter( buf ) );

	// 送るパケットが存在したら送ります。
	// TODO: ここも1024を超過する可能性があるので分割して送ること。
	if ( !buf.empty( ) )
	{
		mSocket.write( mConnectServerHandle, buf.size( ), buf.data( ) );
		buf.clear( );
		buf.shrink_to_fit( );
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

        mCloseSecond = cinder::app::getElapsedSeconds( ) + HOLD_SECOND;

        using namespace Node::Action;
        auto act = repeat_forever::create( sequence::create( delay::create( 1.5F ), call_func::create( [ this ]
        {
            send( new Packet::Deliver::cDliPing( ) );
        } ) ) );
        act->set_name( "ping" );
        mRoot->run_action( act );
    }
    if ( !isConnected( ) )
    {
        if ( mConnectSecond < cinder::app::getElapsedSeconds( ) )
        {
            close( );
            MES_ERR( "サーバーからの応答がありません。",
                     [ ] { cSceneManager::getInstance( )->change<Scene::Member::cTitle>( ); } );
        }
    }
}
void cUDPClientManager::ping( )
{
    while ( auto p = cEventManager::getInstance( )->getEvePing( ) )
    {
        mCloseSecond = cinder::app::getElapsedSeconds( ) + HOLD_SECOND;
    }
    if (mConnectServerHandle.ipAddress != Network::getLocalIpAddressHost())
    {
        if (mCloseSecond < cinder::app::getElapsedSeconds())
        {
            close();
            MES_ERR( "サーバーとの接続が切れました。",
                     [ ] { cSceneManager::getInstance( )->change<Scene::Member::cTitle>( ); } );
        }
    }
}
void cUDPClientManager::sendDataBufferAdd( cPacketBuffer const & packetBuffer, bool reliable )
{
	auto& buf = mSendDataBuffer;

    // パケットが大きくなりそうなら先に送ってしまいます。
    if ( 1024 < buf.size( ) )
    {
		mSocket.write( mConnectServerHandle, buf.size(), buf.data() );
		buf.clear( );
		buf.shrink_to_fit( );
    }

	ubyte2 const& byte = packetBuffer.transferredBytes;
	cBuffer const& buffer = packetBuffer.buffer;

	if ( reliable )
	{
		std::vector<char> buf;
		buf.resize( buf.size( ) + byte );
		memcpy( buf.data( ) + buf.size( ) - byte, &buffer, byte );
		mReliableManager.append( std::move( buf ) );
	}
	else
	{
		buf.resize( buf.size( ) + byte );
		memcpy( buf.data( ) + buf.size( ) - byte, &buffer, byte );
	}
}
}