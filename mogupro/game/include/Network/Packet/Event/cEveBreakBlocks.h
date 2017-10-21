#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <vector>
#include <cinder/Vector.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveBreakBlocks : public cPacketBase<cEveBreakBlocks, PacketId::EVE_BREAK_BLOCKS>
{
public:
    cEveBreakBlocks( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    std::vector<cinder::vec3> mBreakPositions;
};
}
}
}