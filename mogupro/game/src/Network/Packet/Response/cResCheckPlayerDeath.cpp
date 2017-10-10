#include <Network/Packet/Response/cResCheckPlayerDeath.h>
namespace Network
{
namespace Packet
{
namespace Response
{
cResCheckPlayerDeath::cResCheckPlayerDeath( )
{

}
void cResCheckPlayerDeath::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{

}
ubyte2 cResCheckPlayerDeath::packetExport( char* const data )
{
    return 0;
}
}
}
}