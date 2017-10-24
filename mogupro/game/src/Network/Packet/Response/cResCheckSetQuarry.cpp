#include <Network/Packet/Response/cResCheckSetQuarry.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Response
{
void cResCheckSetQuarry::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter( data ) >> mIsSucceeded >> mPosition >> mDrillId >> mType;
}
ubyte2 cResCheckSetQuarry::packetExport( char* const data )
{
    return cExporter( data ) << mIsSucceeded << mPosition << mDrillId << mType;
}
}
}
}