#include <Network/Packet/Request/cReqCheckPlayerDeath.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckPlayerDeath::cReqCheckPlayerDeath( )
{

}
void cReqCheckPlayerDeath::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cReqCheckPlayerDeath::packetExport( char* const data )
{
    return 0;
}
}
}
}