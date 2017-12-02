#include <Network/Packet/Request/cReqCheckPlayerDeath.h>
#include <Network/PackFunction.hpp>
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
	this->networkHandle = networkHandle;
	cImporter imp( data );
	// Žg‚¢•û: vec2 pos1, pos2; ubyte2 a1, a2;
	// imp >> pos1 >> a1 >> pos2 >> a2;
	imp >> playerId >> enemyId;
}
ubyte2 cReqCheckPlayerDeath::packetExport( char* const data )
{
	cExporter exp( data );
	exp << playerId << enemyId;
    return 0;
}
}
}
}