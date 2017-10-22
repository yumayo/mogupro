#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveSetQuarry : public cPacketBase<cEveSetQuarry, PacketId::EVE_SET_QUARRY>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    cinder::vec3 mPosition;
    ubyte1 mType;
    ubyte1 mTeamId;
    ubyte2 mDrillId;
};
}
}
}