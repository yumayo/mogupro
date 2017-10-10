#include <Network/Packet/Event/cEveString.h>
namespace Network
{
namespace Packet
{
namespace Event
{
cEveString::cEveString( )
    : str( )
{

}
cEveString::cEveString( std::string const & str )
    : str( str )
{
}
void cEveString::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    str = std::string( data, transferredBytes );
}
ubyte2 cEveString::packetExport( char* const data )
{
    memcpy( data, str.data( ), str.size( ) );
    return str.size( );
}
}
}
}