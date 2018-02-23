#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveAddCannonPower : public cPacketBase<cEveAddCannonPower, PacketId::EVE_ADD_CANNON_POWER>
{
public:
    cEveAddCannonPower( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
	ubyte1 playerOrQuarry;
	ubyte1 playerId;
	ubyte2 power;
};
}
}
}