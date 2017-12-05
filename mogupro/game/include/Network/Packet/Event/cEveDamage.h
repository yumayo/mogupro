#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveDamage : public cPacketBase<cEveDamage, PacketId::EVE_DAMAGE>
{
public:
    cEveDamage( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	ubyte1 playerId;
	ubyte1 enemyId;
	float damage;
};
}
}
}