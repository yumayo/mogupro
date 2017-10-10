#include <Network/Packet/Response/cResPlayer.h>
namespace Network
{
namespace Packet
{
namespace Response
{
cResPlayer::cResPlayer( )
{

}
void cResPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cResPlayer::packetExport( char* const data )
{
    return 0;
}
}
}
}