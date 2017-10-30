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
    mRoot = Node::node::create( );
    mRoot->set_schedule_update( );
    mIsAccept = false;
    mIdCount = 0;
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
    mHandle.clear( );
	 mRoot->remove_action_by_name( "yumayo.net" );
}
void cUDPServerManager::openAccepter( )
{
    mIsAccept = true;

	 using namespace Node::Action;

	 // yuamyo.net�Ɏ������}�b�`���O�T�[�o�[�ł��邱�Ƃ𑗐M����B
	 auto networkHandle = cNetworkHandle( "yumayo.net", 58632 );
	 mRoot->run_action( repeat_times::create( sequence::create( delay::create( 1.5F ), call_func::create( [ networkHandle, this ]
	 {
		 char s_mes [ ] = "connect";
		 mSocket.write( networkHandle, sizeof( s_mes ), s_mes );
	 } ) ), 3 ) );
	 
	 auto act = repeat_forever::create( sequence::create( delay::create( 1.5F ), call_func::create( [ networkHandle, this ]
	 {
		 char s_mes [ ] = "ping";
		 mSocket.write( networkHandle, sizeof( s_mes ), s_mes );
		 cinder::app::console( ) << "cUDPServerManager: " << "ping to " << "yumayo.net" << std::endl;
	 } ) ) );
	 act->set_name( "yumayo.net" );
	 mRoot->run_action( act );
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
    // ���M������̂�����΃o�b�t�@�[������o���đ���B
    for ( auto& handle : mHandle )
    {
        // �]���Ă���p�P�b�g�𑗂�܂��B
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
	// ��M�������̂�����΃o�b�t�@�[������o���ăp�P�b�g�̕��ʂ��s���B
	while ( !mSocket.emptyChunk( ) )
	{
		auto chunk = mSocket.popChunk( );
		try
		{
			if ( cUDPManager::getInstance( )->isConnectPacket( chunk ) ||
				( mHandle.find( chunk.networkHandle ) != mHandle.end( ) ) )
			{
				cUDPManager::getInstance( )->onReceive( chunk );
			}
			else
			{
				// �R�l�N�V�������m�����Ȃ��܂ܑ��M���Ă����ꍇ�B
			}
		}
		catch ( std::runtime_error e )
		{
			// yumayo.net�Ƃ̒ʐM�B
			if ( std::string( chunk.packetBuffer.buffer.data( ), sizeof( "addr: " ) - 1 ) == "addr: " )
			{
				std::string addr( chunk.packetBuffer.buffer.data( ) + sizeof( "addr: " ) - 1, chunk.packetBuffer.transferredBytes - ( sizeof( "addr: " ) - 1 ) );
				cinder::app::console( ) << "�}�b�`���O�T�[�o�[�̃A�h���X�́A" + addr + "�ł��B" << std::endl;
			}
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

    // �p�P�b�g���傫���Ȃ肻���Ȃ��ɑ����Ă��܂��܂��B
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

        cinder::app::console( ) << p->mNetworkHandle.ipAddress << ":" << p->mNetworkHandle.port << "��";
        cinder::app::console( ) << "cUDPServerManager: " << "connecting..." << (int)mIdCount << std::endl;

        auto itr = mHandle.insert( std::make_pair( p->mNetworkHandle, std::move( cClientInfo( mIdCount ) ) ) );
        if ( itr.second )
        {
            cinder::app::console( ) << p->mNetworkHandle.ipAddress << ":" << p->mNetworkHandle.port << "��";
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
            cinder::app::console( ) << p->mNetworkHandle.ipAddress << ":" << p->mNetworkHandle.port << "��";
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
            itr->second.closeSecond = cinder::app::getElapsedSeconds( ) + HOLD_SECOND;
        }
    }

    for ( auto itr = mHandle.begin( ); itr != mHandle.end( ); )
    {
        // ���[�J���̏ꍇ�̓J�E���g�_�E�������܂���B
        if ( itr->first.ipAddress != Network::getLocalIpAddressHost( ) )
        {
            if ( itr->second.closeSecond < cinder::app::getElapsedSeconds( ) )
            {
                cinder::app::console( ) << itr->first.ipAddress << ":" << itr->first.port << "��";
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
    : closeSecond( cinder::app::getElapsedSeconds( ) + HOLD_SECOND )
{
    if ( idCount == 255 )
    {
        MES_ERR( "�N���C�A���g�̐�������ɒB���܂����B",
                 [ ] { exit( 0 ); } );
    }
    else
    {
        id = idCount;
    }
}
}