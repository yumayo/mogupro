#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEvePlayersRespawn; : public cPacketBase<cEvePlayersRespawn;, PacketId::EVE_PLAYERS_RESPAWN;>
{
public:
    cEvePlayersRespawn;( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}