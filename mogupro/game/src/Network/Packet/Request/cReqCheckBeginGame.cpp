#include <Network/Packet/Request/cReqCheckBeginGame.h>

namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckBeginGame::cReqCheckBeginGame( ) : mNetworkHandle("",0)
{

}

void cReqCheckBeginGame::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	mNetworkHandle = networkHandle;
}

ubyte2 cReqCheckBeginGame::packetExport( char* const data )
{

    return 0;
}
}
}
}