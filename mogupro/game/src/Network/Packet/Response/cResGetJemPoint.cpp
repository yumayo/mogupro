#include <Network/Packet/Response/cResGetJemPoint.h>
namespace Network
{
namespace Packet
{
namespace Response
{
cResGetJemPoint::cResGetJemPoint( )
{

}
void cResGetJemPoint::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cResGetJemPoint::packetExport( char* const data )
{
    return 0;
}
}
}
}