#include <Network/Packet/Request/cReqGetJemSeed.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqGetJemSeed::cReqGetJemSeed( )
{

}
void cReqGetJemSeed::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cReqGetJemSeed::packetExport( char* const data )
{
    return 0;
}
}
}
}