#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqGetJemQuarry : public cPacketBase<cReqGetJemQuarry, PacketId::REQ_GET_JEM_QUARRY>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    cNetworkHandle mNetworkHandle;
    ubyte2 mObjectId;
    ubyte2 mGemId;
};
}
}
}