#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqGetJemPlayer : public cPacketBase<cReqGetJemPlayer, PacketId::REQ_GET_JEM_PLAYER>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    cNetworkHandle mNetworkHandle;
    ubyte1 mPlayerId;
    ubyte2 mGemId;
};
}
}
}