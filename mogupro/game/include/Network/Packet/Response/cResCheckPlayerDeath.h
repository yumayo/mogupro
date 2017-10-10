#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckPlayerDeath : public cPacketBase<cResCheckPlayerDeath, PacketId::RES_CHECK_PLAYER_DEATH>
{
public:
    cResCheckPlayerDeath( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}