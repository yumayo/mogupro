#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEvePlayerDeath : public cPacketBase<cEvePlayerDeath, PacketId::EVE_PLAYER_DEATH>
{
public:
    cEvePlayerDeath( ) = default;
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	cNetworkHandle networkHandle;
	std::string playerId;
	std::string enemyId;
};
}
}
}