#include <Network/Packet/Request/cReqString.h>
#include <Network/PackFunction.hpp>
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
    cImporter( data ) >> str;
}
ubyte2 cReqString::packetExport( char* const data )
{
	cExporter exp( data );
	exp << str;
	return exp.getSize( );
}
}
}
}