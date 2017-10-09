#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckPlayerDeath : public cPacketBase<cReqCheckPlayerDeath, PacketId::REQ_CHECK_PLAYER_DEATH>
{
public:
    cReqCheckPlayerDeath( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}