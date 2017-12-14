#include <Network/Packet/Request/cReqGetJemQuarry.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
void cReqGetJemQuarry::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    mNetworkHandle = networkHandle;
    cImporter(data) >> mObjectId >> mGemId;
}
ubyte2 cReqGetJemQuarry::packetExport( char* const data )
{
	cExporter exp( data );
	exp << mObjectId << mGemId;
    return exp.getSize( );
}
}
}
}