#include <Network/Packet/Response/cResCheckMember.h>
namespace Network
{
namespace Packet
{
namespace Response
{
cResCheckMember::cResCheckMember( )
{

}
void cResCheckMember::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cResCheckMember::packetExport( char* const data )
{
    return 0;
}
}
}
}