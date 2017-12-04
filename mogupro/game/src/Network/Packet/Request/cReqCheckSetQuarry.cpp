#include <Network/Packet/Request/cReqCheckSetQuarry.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
void cReqCheckSetQuarry::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    mNetworkHandle = networkHandle;
    cImporter( data ) >> mPosition >> mPlayerId;
}
ubyte2 cReqCheckSetQuarry::packetExport( char* const data )
{
    return cExporter( data ) << mPosition << mPlayerId;
}
}
}
}