#include <Network/Packet/Response/cResCheckPlayerDeath.h>
#include <Network/PackFunction.hpp>
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
	cImporter( data ) >> playerId >> enemyId >> isSuccess;
}
ubyte2 cResCheckPlayerDeath::packetExport( char* const data )
{
    return cExporter(data) << playerId << enemyId << isSuccess;
}
}
}
}