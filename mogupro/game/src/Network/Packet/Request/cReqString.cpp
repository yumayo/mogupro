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
void cReqString::packetImport( ubyte2 size, char const* const data )
{
    str = data;
}
ubyte2 cReqString::packetExport( char* const data )
{
    memcpy( data, str.data( ), str.size( ) );
    return str.size( );
}
}
}
}