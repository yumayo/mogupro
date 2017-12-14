#include <Network/Packet/Request/cReqRespawn.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqRespawn::cReqRespawn( )
{

}
void cReqRespawn::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	this->networkHandle = networkHandle;
    cImporter imp( data );
    // Žg‚¢•û: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
	imp >> playerId;
}
ubyte2 cReqRespawn::packetExport( char* const data )
{
    cExporter exp( data );
    // Žg‚¢•û: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
	exp << playerId;
    return exp.getSize( );
}
}
}
}