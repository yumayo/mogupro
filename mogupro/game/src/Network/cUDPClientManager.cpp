#include <Network/cUDPClientManager.h>
#include <cinder/app/App.h>
#include <limits>
#include <Utility/MessageBox.h>
#include <Network/cUDPManager.h>
#include <Scene/cSceneManager.h>
#include <Scene/Member/cTitle.h>
#include <Node/action.hpp>
#include <Network/IpHost.h>
#include <Network/cUDPServerManager.h>
namespace Network
{
cUDPClientManager::cUDPClientManager( )
    : mCloseSecond( std::numeric_limits<float>::max( ) )
    , mRoot( Node::node::create( ) )
    //, mConnectSecond( std::numeric_limits<float>::max( ) )
	, mSequenceId( 0U )
	, mIsConnected(false)
	, mServerTime( )
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
    return mIsConnected;
}
void cUDPClientManager::connect( std::string const& ipAddress )
{
	mConnectServerHandle = cNetworkHandle(ipAddress, 25565);
	send(new Packet::Request::cReqConnect(), false);
	for (int i = 0; i < 10; ++i)
	{
		std::this_thread::sleep_for(std::chrono::milliseconds(100));
		update(0.1F);
		if (ipAddress == Network::getLocalIpAddressHost())
		{
			cUDPServerManager::getInstance()->update(0.1F);
		}
		connection();
		if (isConnected())
		{
			break;
		}
	}
	if (!isConnected())
	{
		close();
		// サーバーからの応答なし。
		Scene::cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
	}
	//mConnectSecond = cinder::app::getElapsedSeconds( ) + PING_HOLD_SECOND;
}
void cUDPClientManager::connectOfflineServer( )
{
    connect( Network::getLocalIpAddressHost( ) );
}
void cUDPClientManager::update( float delta )
{
	if ( isConnected( ) )
	{
		mServerTime += delta;
	}
    updateRecv( );
    updateSend( );
    mRoot->entry_update( delta );
}
void cUDPClientManager::setDontClose(bool value)
{
	mStopClose = value;
}
float const & cUDPClientManager::getServerTime( )
{
	return mServerTime;
}
void cUDPClientManager::updateSend( )
{
	if ( !mConnectServerHandle )
	{
		close( );

		// 送信ハンドルが未定義です。
		Scene::cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
	}

	auto& handle = mConnectServerHandle;
	auto& buf = mSendDataBuffer;

	// リライアブルなデータを詰めます。
	auto reliableData = mReliableManager.update( );
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
		mPackets.onReceive( chunk );
    }

    ping( );
}
void cUDPClientManager::connection( )
{
    while ( auto p = mPackets.ResConnect.get( ) )
    {
		mIsConnected = true;

        mCloseSecond = cinder::app::getElapsedSeconds( ) + PING_HOLD_SECOND;

		mServerTime = p->time;

        using namespace Node::Action;
        auto act = repeat_forever::create( sequence::create( delay::create( 0.25F ), call_func::create( [ this ]
        {
			send( new Packet::Request::cReqPing( ) );
        } ) ) );
        act->set_name( "ping" );
        mRoot->run_action( act );
    }
   // if ( !isConnected( ) )
   // {
   //     if ( mConnectSecond < cinder::app::getElapsedSeconds( ) )
   //     {
   //         close( );
			//// サーバーからの応答なし。
			//Scene::cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
   //     }
   // }
}
void cUDPClientManager::ping( )
{
    while ( auto p = mPackets.EvePing.get( ) )
    {
        mCloseSecond = cinder::app::getElapsedSeconds( ) + PING_HOLD_SECOND;

		mServerTime = p->time;
    }
    //if (mConnectServerHandle.ipAddress != Network::getLocalIpAddressHost())
    {
		if (!mStopClose)
		{
			if (mCloseSecond < cinder::app::getElapsedSeconds())
			{
				close();
				// サーバーからの接続切れ。
				Scene::cSceneManager::getInstance()->shift<Scene::Member::cTitle>();
			}
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