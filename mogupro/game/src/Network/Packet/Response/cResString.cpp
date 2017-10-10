#include <Network/Packet/Response/cResString.h>
namespace Network
{
namespace Packet
{
namespace Response
{
cResString::cResString( )
    : str( )
{

}
cResString::cResString( std::string str )
    : str( str )
{
}
void cResString::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    str = std::string( data, transferredBytes );
}
ubyte2 cResString::packetExport( char* const data )
{
    memcpy( data, str.data( ), str.size( ) );
    return str.size( );
}
}
}
}