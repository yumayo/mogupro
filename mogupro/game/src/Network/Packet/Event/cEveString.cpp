#include <Network/Packet/Event/cEveString.h>
#include <Network/PackFunction.hpp>
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
    cImporter( data ) >> str;
}
ubyte2 cEveString::packetExport( char* const data )
{
	cExporter exp( data );
	exp << str;
    return exp.getSize( );
}
}
}
}