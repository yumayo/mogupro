#include <Network/Packet/Request/cReqCheckGetJemQuarry.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
void cReqCheckGetJemQuarry::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    mNetworkHandle = networkHandle;
    cImporter(data) >> mObjectId >> mGemId;
}
ubyte2 cReqCheckGetJemQuarry::packetExport( char* const data )
{
    return  cExporter( data ) << mObjectId << mGemId;
}
}
}
}