#include <Network/Packet/Response/cResCheckGetJemQuarry.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Response
{
void cResCheckGetJemQuarry::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter( data ) >> mIsSuccessed >> mObjectId >> mGemId;
}
ubyte2 cResCheckGetJemQuarry::packetExport( char* const data )
{
    return cExporter( data ) << mIsSuccessed << mObjectId << mGemId;
}
}
}
}