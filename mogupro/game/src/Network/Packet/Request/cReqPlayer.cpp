#include <Network/Packet/Request/cReqPlayer.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqPlayer::cReqPlayer( )
{

}
void cReqPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cReqPlayer::packetExport( char* const data )
{
    return 0;
}
}
}
}