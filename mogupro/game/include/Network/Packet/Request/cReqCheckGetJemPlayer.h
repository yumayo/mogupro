#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckGetJemPlayer : public cPacketBase<cReqCheckGetJemPlayer, PacketId::REQ_CHECK_GET_JEM_PLAYER>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    ubyte2 mGemId;
};
}
}
}