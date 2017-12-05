#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqDamage : public cPacketBase<cReqDamage, PacketId::REQ_DAMAGE>
{
public:
    cReqDamage( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	ubyte1 enemyId;
	ubyte1 playerId;
	float damage;
};
}
}
}