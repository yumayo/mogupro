#include <Network/Packet/Event/cEvePlayerDeath.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Event
{
void cEvePlayerDeath::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	this->networkHandle = networkHandle;
	cImporter imp( data );
	imp >> playerId >> enemyId;
}
ubyte2 cEvePlayerDeath::packetExport( char* const data )
{
	cExporter exp( data );
	exp << playerId << enemyId;
	return exp;
}
}
}
}