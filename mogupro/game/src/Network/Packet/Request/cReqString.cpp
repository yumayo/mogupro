#include <Network/Packet/Request/cReqString.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqString::cReqString( )
    : str( )
{

}
cReqString::cReqString( std::string str )
    : str( str )
{
}
void cReqString::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    str = std::string( data, transferredBytes );
}
ubyte2 cReqString::packetExport( char* const data )
{
    memcpy( data, str.data( ), str.size( ) );
    return str.size( );
}
}
}
}