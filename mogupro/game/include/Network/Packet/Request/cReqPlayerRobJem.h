#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqPlayerRobJem : public cPacketBase<cReqPlayerRobJem, PacketId::REQ_PLAYER_ROB_JEM>
{
public:
    cReqPlayerRobJem( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}