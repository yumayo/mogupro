#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveGetJemQuarry : public cPacketBase<cEveGetJemQuarry, PacketId::EVE_GET_JEM_QUARRY>
{
public:
    cEveGetJemQuarry( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    ubyte2 mObjectId;
    ubyte2 mGemId;
};
}
}
}