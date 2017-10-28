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
    : mRoot( Node::node::create( ) )
{
    mRoot->set_schedule_update( );
    mIsAccept = false;
    mIdCount = 0;
}
void cUDPServerManager::close( )
{
    mSocket.close( );
    mIsAccept = false;
}
void cUDPServerManager::open( )
{
    mSocket.open( 25565 );
    mIsAccept = true;
}
void cUDPServerManager::closeAccepter( )
{
    mIsAccept = false;
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
    auto itr = mHandle.find( handle );
    if ( itr != mHandle.end( ) )
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
    for ( auto& handle : mHandle )
    {
        // 余ってたらパケットを送ります。
        if ( !handle.second.buffer.empty( ) )
        {
            mSocket.write( handle.first, handle.second.buffer.size( ), handle.second.buffer.data( ) );
            handle.second.buffer.clear( );
            handle.second.buffer.shrink_to_fit( );
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
            ( mHandle.find( chunk.networkHandle ) != mHandle.end( ) ) )
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
void cUDPServerManager::sendDataBufferAdd( cNetworkHandle const & networkHandle, cPacketBuffer const & packetBuffer )
{
    auto itr = mHandle.find( networkHandle );
    if ( itr == mHandle.end( ) ) return;

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

    buf.resize( buf.size( ) + byte );
    memcpy( buf.data( ) + buf.size( ) - byte, &buffer, byte );
}
void cUDPServerManager::connection( )
{
    while ( auto p = cRequestManager::getInstance( )->getReqConnect( ) )
    {
        if ( !mIsAccept ) continue;

        cinder::app::console( ) << p->mNetworkHandle.ipAddress << ":" << p->mNetworkHandle.port << "＠";
        cinder::app::console( ) << "cUDPServerManager: " << "connecting..." << (int)mIdCount << std::endl;

        auto itr = mHandle.insert( std::make_pair( p->mNetworkHandle, std::move( cClientInfo( mIdCount ) ) ) );
        if ( itr.second )
        {
            cinder::app::console( ) << p->mNetworkHandle.ipAddress << ":" << p->mNetworkHandle.port << "＠";
            cinder::app::console( ) << "cUDPServerManager: " << "connect success!! " << (int)mIdCount << std::endl;

            mIdCount += 1;

            send( p->mNetworkHandle, new Packet::Response::cResConnect( ) );

            using namespace Node::Action;
            auto networkHandle = p->mNetworkHandle;
            auto act = repeat_forever::create( sequence::create( delay::create( 1.5F ), call_func::create( [ networkHandle, this ]
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
        auto itr = mHandle.find( p->mNetworkHandle );
        if ( itr != mHandle.end( ) )
        {
            itr->second.closeSecond = cinder::app::getElapsedSeconds( ) + 60.0F;
        }
    }

    for ( auto itr = mHandle.begin( ); itr != mHandle.end( ); )
    {
        // ローカルの場合はカウントダウンをしません。
        if ( itr->first.ipAddress != Network::getLocalIpAddressHost( ) )
        {
            if ( itr->second.closeSecond < cinder::app::getElapsedSeconds( ) )
            {
                cinder::app::console( ) << itr->first.ipAddress << ":" << itr->first.port << "＠";
                cinder::app::console( ) << "cUDPServerManager: " << "disconnect" << itr->second.id << std::endl;
                mRoot->remove_action_by_tag( itr->second.id );
                mHandle.erase( itr++ );
                continue;
            }
        }
        itr++;
    }
}
cUDPServerManager::cClientInfo::cClientInfo( ubyte1 idCount )
    : closeSecond( cinder::app::getElapsedSeconds( ) + 60.0F )
{
    if ( idCount == 255 )
    {
        MES_ERR( "クライアントの数が上限に達しました。",
                 [ ] { exit( 0 ); } );
    }
    else
    {
        id = idCount;
    }
}
}