#include <Network/Packet/Request/cReqPlayerAttack.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqPlayerAttack::cReqPlayerAttack( )
{

}
void cReqPlayerAttack::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    this->networkHandle = networkHandle;
    cImporter packet( data );
    // Žg‚¢•û: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet >> pos1 >> a1 >> pos2 >> a2;
	packet >> playerId >> call;
}
ubyte2 cReqPlayerAttack::packetExport( char* const data )
{
    cExporter packet( data );
    // Žg‚¢•û: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet << pos1 << a1 << pos2 << a2;
	packet << playerId << call;
    return packet;
}
}
}
}