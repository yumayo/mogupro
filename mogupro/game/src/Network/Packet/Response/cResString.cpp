#include <Network/Packet/Response/cResString.h>
#include <Network/PackFunction.hpp>
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
    cImporter( data ) >> str;
}
ubyte2 cResString::packetExport( char* const data )
{
	cExporter exp( data );
	exp << str;
	return exp.getSize( );
}
}
}
}