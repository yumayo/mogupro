#include <Network/Packet/Response/cResWantTeamIn.h>
namespace Network
{
namespace Packet
{
namespace Response
{
cResWantTeamIn::cResWantTeamIn( )
{

}
void cResWantTeamIn::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cResWantTeamIn::packetExport( char* const data )
{
    return 0;
}
}
}
}