#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEvePlayerAttack : public cPacketBase<cEvePlayerAttack, PacketId::EVE_PLAYER_ATTACK>
{
public:
    cEvePlayerAttack( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
	ubyte1 playerId;
	ubyte1 call;
};
}
}
}