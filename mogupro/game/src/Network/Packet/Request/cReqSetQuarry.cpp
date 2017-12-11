#include <Network/Packet/Request/cReqSetQuarry.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
void cReqSetQuarry::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    mNetworkHandle = networkHandle;
    cImporter( data ) >> mPosition >> mPlayerId;
}
ubyte2 cReqSetQuarry::packetExport( char* const data )
{
    return cExporter( data ) << mPosition << mPlayerId;
}
}
}
}