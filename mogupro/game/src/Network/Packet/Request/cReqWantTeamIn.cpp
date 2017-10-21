#include <Network/Packet/Request/cReqWantTeamIn.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqWantTeamIn::cReqWantTeamIn( )
{

}
void cReqWantTeamIn::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cReqWantTeamIn::packetExport( char* const data )
{
    return 0;
}
}
}
}