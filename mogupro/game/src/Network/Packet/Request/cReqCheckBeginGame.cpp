#include <Network/Packet/Request/cReqCheckBeginGame.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckBeginGame::cReqCheckBeginGame( )
{

}
void cReqCheckBeginGame::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cReqCheckBeginGame::packetExport( char* const data )
{
    return 0;
}
}
}
}