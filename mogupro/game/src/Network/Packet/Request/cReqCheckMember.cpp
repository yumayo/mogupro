#include <Network/Packet/Request/cReqCheckMember.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckMember::cReqCheckMember( )
{

}
void cReqCheckMember::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cReqCheckMember::packetExport( char* const data )
{
    return 0;
}
}
}
}