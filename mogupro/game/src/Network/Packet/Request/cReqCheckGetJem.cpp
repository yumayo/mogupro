#include <Network/Packet/Request/cReqCheckGetJem.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckGetJem::cReqCheckGetJem( )
{

}
void cReqCheckGetJem::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cReqCheckGetJem::packetExport( char* const data )
{
    return 0;
}
}
}
}