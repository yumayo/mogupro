#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckPlayerRobJem : public cPacketBase<cReqCheckPlayerRobJem, PacketId::REQ_CHECK_PLAYER_ROB_JEM>
{
public:
    cReqCheckPlayerRobJem( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}