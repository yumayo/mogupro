#include <Network/Packet/Response/cResCheckBeginGame.h>
namespace Network
{
namespace Packet
{
namespace Response
{
cResCheckBeginGame::cResCheckBeginGame( )
{

}
void cResCheckBeginGame::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cResCheckBeginGame::packetExport( char* const data )
{
    return 0;
}
}
}
}