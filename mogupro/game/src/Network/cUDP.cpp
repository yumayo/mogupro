#include <Network/cUDP.h>
#include <boost/lexical_cast.hpp>
#include <Utility/MessageBox.h>
using udp = boost::asio::ip::udp;
namespace Network
{
cUDP::cUDP( )
    : mUdpSocket( mIoService )
{
    mRemoteBuffer.fill( 0 );
}
cUDP::~cUDP( )
{
	close( );
}
void cUDP::write( cNetworkHandle const& networkHandle, size_t sendDataByteNumber, char const * sendData )
{
    try
    {
        udp::resolver resolver( mIoService );
        udp::resolver::query query( udp::v4( ),
                                    networkHandle.ipAddress,
                                    boost::lexical_cast<std::string>( networkHandle.port ) );
        mUdpSocket.send_to( boost::asio::buffer( sendData, sendDataByteNumber ),
                            resolver.resolve( query )->endpoint( ) );
    }
    catch ( boost::system::error_code& e )
    {
        // 送信できない時に呼ばれます。
        // ネットワークデバイスが無効になっているときや、
        // ネットワークにつながっていないときに呼ばれます。
        MES_ERR( e.message( ),
                 [ ] { } );
    }
}
void cUDP::close( )
{
	if ( mUdpSocket.is_open( ) )
	{
		mIsPause = true;
		mUdpSocket.close( );
		mIoService.stop( );
		mThreadIoService.join( );
	}
}
void cUDP::open( )
{
    open( 0 );
}
void cUDP::open( int port )
{
    mIsPause = false;
    if ( !mUdpSocket.is_open( ) )
    {
        boost::system::error_code e;
        mUdpSocket.open( udp::v4( ), e );
        if ( e )
        {
            // openできなかった時
            MES_ERR( e.message( ),
                     [ ] { } );
        }
        mUdpSocket.bind( udp::endpoint( udp::v4( ), port ), e );
        if ( e )
        {
            // bindできなかった時
            MES_ERR( e.message( ),
                     [ ] { } );
        }
    }
    mThreadIoService = std::thread( [ this ]
    {
        while ( !mIsPause )
        {
            receive( );
            mIoService.run( );
            mIoService.reset( );
        }
    } );
}
void cUDP::clearChunk( )
{
	std::lock_guard<std::mutex> m( mDataMutex );
    mCacheEndpoints.clear( );
    mCacheEndpoints.shrink_to_fit( );
}
bool cUDP::emptyChunk( )
{
	std::lock_guard<std::mutex> m( mDataMutex );
    return mCacheEndpoints.empty( );
}
cPacketChunk cUDP::popChunk( )
{
	std::lock_guard<std::mutex> m( mDataMutex );
    auto front = mCacheEndpoints.front( );
    mCacheEndpoints.pop_front( );
    return front;
}
void cUDP::receive( )
{
	std::lock_guard<std::mutex> m( mDataMutex );
    mUdpSocket.async_receive_from( boost::asio::buffer( mRemoteBuffer ),
                                   mRemoteEndpoint,
                                   [ this ] ( const boost::system::error_code& e, size_t transferredBytes )
    {
        if ( e )
        {
            // 受信できなかった時に呼ばれます。
            // 外側からソケットを閉じられたときなど。
            //MES_ERR( e.message( ),
            //         [ ] { } );
        }
        else
        {
			std::lock_guard<std::mutex> m( mDataMutex );
            mCacheEndpoints.emplace_back( mRemoteEndpoint.address( ).to_string( ), mRemoteEndpoint.port( ), transferredBytes, mRemoteBuffer );
            std::fill_n( mRemoteBuffer.begin( ), transferredBytes, 0 );
        }
    } );
}
}