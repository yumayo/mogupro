#include <Network/Packet/Response/cResGetJemSeed.h>
namespace Network
{
namespace Packet
{
namespace Response
{
cResGetJemSeed::cResGetJemSeed( )
{

}
void cResGetJemSeed::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cResGetJemSeed::packetExport( char* const data )
{
    return 0;
}
}
}
}