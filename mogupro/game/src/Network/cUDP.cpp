#include <Network/cUDPManager.h>
#include <Utility/cScopedMutex.h>
#include <boost/lexical_cast.hpp>
using udp = boost::asio::ip::udp;
namespace Network
{
cUDP::cUDP( )
    : cUDP( 0 )
{
}
cUDP::cUDP( int port )
    : mUdpSocket( mIoService )
{
    mRemoteBuffer.fill( 0 );
}
void cUDP::write( std::string ipaddress, int port, char const * sendData, size_t sendDataByteNumber )
{
    try
    {
        udp::resolver resolver( mIoService );
        udp::resolver::query query( udp::v4( ),
                                    ipaddress,
                                    boost::lexical_cast<std::string>( port ) );
        mUdpSocket.send_to( boost::asio::buffer( sendData, sendDataByteNumber ),
                            resolver.resolve( query )->endpoint( ) );
    }
    catch ( boost::system::error_code& error )
    {
        // ëóÇÍÇ‹ÇπÇÒÇ≈ÇµÇΩÅB
    }
}
void cUDP::close( )
{
    mIsPause = true;
    mIoService.stop( );
    mUpdateIoService.join( );
    mUdpSocket.close( );
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
        mUdpSocket.open( udp::v4( ) );
        mUdpSocket.bind( udp::endpoint( udp::v4( ), port ) );
    }
    mUpdateIoService = std::thread( [ this ]
    {
        while ( !mIsPause )
        {
            receive( );
            mIoService.run( );
            mIoService.reset( );
        }
    } );
}
void cUDP::receive( )
{
    mUdpSocket.async_receive_from( boost::asio::buffer( mRemoteBuffer ),
                                   mRemoteEndpoint,
                                   [ this ] ( const boost::system::error_code& e, size_t bytes_transferred )
    {
        if ( e )
        {
            int a = 0;
        }
        else
        {
            Utility::cScopedMutex m( mMutex );
            mReceiveBuffers[mRemoteEndpoint].emplace_back( mRemoteBuffer );
            std::fill_n( mRemoteBuffer.begin( ), bytes_transferred, 0 );
        }
    } );
}
}