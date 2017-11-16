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
cResSetGamestartTimer::cResSetGamestartTimer(std::string timerStr):mTimerStr(timerStr)
{

}

void cResSetGamestartTimer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
	imp >> mTimerStr;
}
ubyte2 cResSetGamestartTimer::packetExport( char* const data )
{
    cExporter exp( data );
	exp << mTimerStr;
    return exp;
}
}
}
}