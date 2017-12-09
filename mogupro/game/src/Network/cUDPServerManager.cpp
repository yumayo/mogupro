#include <Network/cUDPServerManager.h>
#include <Network/cUDPManager.h>
#include <Network/cDeliverManager.h>
#include <Network/cEventManager.h>
#include <Network/cRequestManager.h>
#include <Network/cResponseManager.h>
#include <cinder/app/App.h>
#include <limits>
#include <Node/action.hpp>
#include <Utility/MessageBox.h>
#include <Network/IpHost.h>
namespace Network
{
cUDPServerManager::cUDPServerManager( )
{
	closeAccepter( );
	mRoot = Node::node::create( );
	mRoot->set_schedule_update( );
}
void cUDPServerManager::close( )
{
    mRoot.reset( );
    mSocket.close( );
    closeAccepter( );
}
void cUDPServerManager::open( )
{
    mRoot = Node::node::create( );
    mRoot->set_schedule_update( );
    mSocket.open( 25565 );
    openAccepter( );
}
void cUDPServerManager::closeAccepter( )
{
	mIsAccept = false;
	mIdCount = 0;
	mConnections.clear( );
}
void cUDPServerManager::openAccepter( )
{
    mIsAccept = true;
}
void cUDPServerManager::update( float delta )
{
    updateRecv( );
    updateSend( );
    mRoot->entry_update( delta );
}
ubyte1 cUDPServerManager::getPlayerId( cNetworkHandle const & handle )
{
    auto itr = mConnections.find( handle );
    if ( itr != mConnections.end( ) )
    {
        return itr->second.id;
    }
    else
    {
        throw std::runtime_error( "Networkhandle nothing" );
    }
}
void cUDPServerManager::updateSend( )
{
    // 送信するものがあればバッファーから取り出して送る。
    for ( auto& client : mConnections )
    {
		auto& handle = client.first;
		auto& buf = client.second.buffer;

		// リライアブルなデータを詰めます。
		auto reliableData = client.second.reliableManager.update( );
		std::copy( reliableData.begin(), reliableData.end(), std::back_inserter( buf ) );

        // 余ってたらパケットを送ります。
        if ( !buf.empty( ) )
        {
			mSocket.write( handle, buf.size( ), buf.data( ) );
            buf.clear( );
            buf.shrink_to_fit( );
        }
    }
}
void cUDPServerManager::updateRecv( )
{
    // 受信したものがあればバッファーから取り出してパケットの分別を行う。
    while ( !mSocket.emptyChunk( ) )
    {
        auto chunk = mSocket.popChunk( );
        if ( cUDPManager::getInstance( )->isConnectPacket( chunk ) ||
            ( mConnections.find( chunk.networkHandle ) != mConnections.end( ) ) )
        {
            cUDPManager::getInstance( )->onReceive( chunk );
        }
        else
        {
            // コネクションを確立しないまま送信してきた場合。
        }
    }

    connection( );
    ping( );
}
void cUDPServerManager::sendDataBufferAdd( cNetworkHandle const & networkHandle, cPacketBuffer const & packetBuffer, bool reliable )
{
    auto itr = mConnections.find( networkHandle );
    if ( itr == mConnections.end( ) ) return;

    auto& buf = itr->second.buffer;

	// パケットが大きくなりそうなら先に送ってしまいます。
	if ( 1024 < buf.size( ) )
	{
		mSocket.write( networkHandle, buf.size( ), buf.data( ) );
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
		itr->second.reliableManager.append( std::move( buf ) );
	}
	else
	{
		buf.resize( buf.size( ) + byte );
		memcpy( buf.data( ) + buf.size( ) - byte, &buffer, byte );
	}
}
void cUDPServerManager::connection( )
{
    while ( auto p = cRequestManager::getInstance( )->getReqConnect( ) )
    {
        if ( !mIsAccept ) continue;

        cinder::app::console( ) << p->mNetworkHandle.ipAddress << ":" << p->mNetworkHandle.port << "＠";
        cinder::app::console( ) << "cUDPServerManager: " << "connecting..." << (int)mIdCount << std::endl;

        auto itr = mConnections.insert( std::make_pair( p->mNetworkHandle, std::move( cConnectionInfo( mIdCount ) ) ) );
        if ( itr.second )
        {
            cinder::app::console( ) << p->mNetworkHandle.ipAddress << ":" << p->mNetworkHandle.port << "＠";
            cinder::app::console( ) << "cUDPServerManager: " << "connect success!! " << (int)mIdCount << std::endl;

            mIdCount += 1;

            send( p->mNetworkHandle, new Packet::Response::cResConnect( ), false );

			// pingコルーチンを走らせる。
            using namespace Node::Action;
            auto act = repeat_forever::create( sequence::create( delay::create( 1.5F ), call_func::create( [ networkHandle = p->mNetworkHandle, this ]
            {
                send( networkHandle, new Packet::Event::cEvePing( ) );
                cinder::app::console( ) << "cUDPServerManager: " << "ping to " << (int)getPlayerId( networkHandle ) << std::endl;
            } ) ) );
            act->set_tag( itr.first->second.id );
            mRoot->run_action( act );
        }
        else
        {
            cinder::app::console( ) << p->mNetworkHandle.ipAddress << ":" << p->mNetworkHandle.port << "＠";
            cinder::app::console( ) << "cUDPServerManager: " << "connect faild " << (int)mIdCount << std::endl;
        }
    }
}
void cUDPServerManager::ping( )
{
    while ( auto p = cDeliverManager::getInstance( )->getDliPing( ) )
    {
        auto itr = mConnections.find( p->mNetworkHandle );
        if ( itr != mConnections.end( ) )
        {
            itr->second.closeSecond = cinder::app::getElapsedSeconds( ) + PING_HOLD_SECOND;
        }
    }

    for ( auto itr = mConnections.begin( ); itr != mConnections.end( ); )
    {
        // ローカルの場合はカウントダウンをしません。
        if ( itr->first.ipAddress != Network::getLocalIpAddressHost( ) )
        {
            if ( itr->second.closeSecond < cinder::app::getElapsedSeconds( ) )
            {
                cinder::app::console( ) << itr->first.ipAddress << ":" << itr->first.port << "＠";
                cinder::app::console( ) << "cUDPServerManager: " << "disconnect" << itr->second.id << std::endl;
                mRoot->remove_action_by_tag( itr->second.id );
                mConnections.erase( itr++ );
                continue;
            }
        }
        ++itr;
    }
}
}