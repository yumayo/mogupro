#include <Network/Packet/Response/cResSetGamestartTimer.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Response
{
cResSetGamestartTimer::cResSetGamestartTimer( )
{

}
cResSetGamestartTimer::cResSetGamestartTimer(float time) : time(time)
{

}

void cResSetGamestartTimer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
	imp >> time;
}
ubyte2 cResSetGamestartTimer::packetExport( char* const data )
{
    cExporter exp( data );
	exp << time;
    return exp.getSize( );
}
}
}
}