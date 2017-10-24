#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckSetQuarry : public cPacketBase<cResCheckSetQuarry, PacketId::RES_CHECK_SET_QUARRY>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    ubyte1 mIsSucceeded;
    cinder::vec3 mPosition;
    ubyte1 mType;
    ubyte2 mDrillId;
};
}
}
}