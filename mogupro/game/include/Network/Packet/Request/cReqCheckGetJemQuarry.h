#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckGetJemQuarry : public cPacketBase<cReqCheckGetJemQuarry, PacketId::REQ_CHECK_GET_JEM_QUARRY>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    cNetworkHandle mNetworkHandle;
    ubyte2 mDrillId;
    ubyte2 mGemId;
};
}
}
}