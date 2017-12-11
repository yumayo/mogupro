#include <Network/Packet/Request/cReqEndGamemainSetup.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqEndGamemainSetup::cReqEndGamemainSetup( )
{

}
void cReqEndGamemainSetup::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	mNetworkHandle = networkHandle;
}
ubyte2 cReqEndGamemainSetup::packetExport( char* const data )
{
    cExporter exp( data );
    return exp;
}
}
}
}