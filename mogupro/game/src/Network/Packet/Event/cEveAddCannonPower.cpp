#include <Network/Packet/Event/cEveAddCannonPower.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Event
{
cEveAddCannonPower::cEveAddCannonPower( )
{

}
void cEveAddCannonPower::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    this->networkHandle = networkHandle;
    cImporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet >> pos1 >> a1 >> pos2 >> a2;
	packet >> playerOrQuarry >> playerId >> power;
}
ubyte2 cEveAddCannonPower::packetExport( char* const data )
{
    cExporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet << pos1 << a1 << pos2 << a2;
	packet << playerOrQuarry << playerId << power;
    return packet.getSize( );
}
}
}
}