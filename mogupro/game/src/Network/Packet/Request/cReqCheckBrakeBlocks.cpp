#include <Network/Packet/Request/cReqCheckBrakeBlocks.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckBrakeBlocks::cReqCheckBrakeBlocks( )
{

}
void cReqCheckBrakeBlocks::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cReqCheckBrakeBlocks::packetExport( char* const data )
{
    return 0;
}
}
}
}