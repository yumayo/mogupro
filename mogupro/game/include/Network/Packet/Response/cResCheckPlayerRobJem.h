#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
#pragma pack(1)
class cResCheckPlayerRobJem : public cPacketBase<cResCheckPlayerRobJem, PacketId::RES_CHECK_PLAYER_ROB_JEM>
{
public:
    cResCheckPlayerRobJem( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
#pragma pack()
}
}
}