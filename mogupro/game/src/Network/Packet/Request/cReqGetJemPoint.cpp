#include <Network/Packet/Request/cReqGetJemPoint.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqGetJemPoint::cReqGetJemPoint( )
{

}
void cReqGetJemPoint::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cReqGetJemPoint::packetExport( char* const data )
{
    return 0;
}
}
}
}