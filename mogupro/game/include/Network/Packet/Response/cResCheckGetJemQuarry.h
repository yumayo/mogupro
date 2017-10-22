#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckGetJemQuarry : public cPacketBase<cResCheckGetJemQuarry, PacketId::RES_CHECK_GET_JEM_QUARRY>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    ubyte1 mIsSuccessed;
    ubyte2 mDrillId;
    ubyte2 mGemId;
};
}
}
}