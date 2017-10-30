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
	 , mFoundMatchingServer( false )
{
    mRoot->set_schedule_update( );
	 mOnFoundMatchingServer = [ this ](std::string const& addr)
	 {
		 if ( addr == "null" )
		 {
			 MES_ERR( "マッチングサーバーが見つかりませんでした。", [ ] { } );
		 }
		 else
		 {
			 if ( !mFoundMatchingServer )
			 {
				 connect( addr );
			 }
			 mFoundMatchingServer = true;
		 }
	 };
}
void cUDPClientManager::close( )
{
    mSocket.close( );
    mRoot->remove_action_by_name( "ping" );
	 mFoundMatchingServer = false;
}
void cUDPClientManager::open( )
{
    mSocket.open( );
}
bool cUDPClientManager::isConnected( )
{
    return mConnectServerHandle;
}
void cUDPClientManager::connectMatchingServer( )
{
	// yuamyo.netにマッチングサーバーが誰だと聞く。
	using namespace Node::Action;
	mRoot->run_action( sequence::create( repeat_times::create( sequence::create( delay::create( 1.5F ), call_func::create( [ this ]
	{
		char s_mes [ ] = "whois";
		auto networkHandle = cNetworkHandle( "yumayo.net", 58632 );
		mSocket.write( networkHandle, sizeof( s_mes ), s_mes );
	} ) ), 3 ), call_func::create( [ this ] 
	{
		if( !mFoundMatchingServer )
			MES_ERR( "マッチングサーバーが見つかりませんでした。", [ ] { } );
	} ) ) );
}
void cUDPClientManager::connect( std::string const& ipAddress )
{
    using namespace Node::Action;
    mRoot->run_action( repeat_times::create( sequence::create( delay::create( 1.5F ), call_func::create( [ this, ipAddress ]
    {
        auto packet = new Packet::Request::cReqConnect( );
        cPacketBuffer packetBuffer;
        packet->createPacket( packetBuffer );
        mSocket.write( cNetworkHandle( ipAddress, 25565 ), packetBuffer.transferredBytes, packetBuffer.buffer.data( ) );
        delete packet;
        mConnectSecond = cinder::app::getElapsedSeconds( ) + HOLD_SECOND;
    } ) ), 3 ) );
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
		try
		{
			cUDPManager::getInstance( )->onReceive( chunk );
		}
		catch ( std::runtime_error e )
		{
			// yumayo.netとの通信。
			if ( std::string( chunk.packetBuffer.buffer.data( ), sizeof( "addr: " ) - 1 ) == "addr: " )
			{
				std::string addr( chunk.packetBuffer.buffer.data( ) + sizeof( "addr: " ) - 1, chunk.packetBuffer.transferredBytes - ( sizeof( "addr: " ) - 1 ) );
				mOnFoundMatchingServer( addr );
				cinder::app::console( ) << "マッチングサーバーのアドレスは、" + addr + "です。" << std::endl;
			}
		}
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
void cUDPClientManager::sendDataBufferAdd( cPacketBuffer const & packetBuffer )
{
    if ( !isConnected( ) )
    {
        close( );
        MES_ERR( "connectが成立する前に通信をしないでください。",
                 [ ] { cSceneManager::getInstance( )->change<Scene::Member::cTitle>( ); } );
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