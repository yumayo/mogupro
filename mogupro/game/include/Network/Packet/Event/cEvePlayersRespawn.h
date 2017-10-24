#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <vector>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEvePlayersRespawn : public cPacketBase<cEvePlayersRespawn, PacketId::EVE_PLAYERS_RESPAWN>
{
public:
    cEvePlayersRespawn( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    std::vector<cinder::vec3> mSpawnPositions;
};
}
}
}