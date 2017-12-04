#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckSetQuarry : public cPacketBase<cReqCheckSetQuarry, PacketId::REQ_CHECK_SET_QUARRY>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    cNetworkHandle mNetworkHandle;
    cinder::vec3 mPosition;
    ubyte1 mPlayerId;
};
}
}
}