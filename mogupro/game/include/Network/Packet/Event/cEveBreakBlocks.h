#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <Network/Packet/StructureDefines.h>
#include <vector>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveBreakBlocks : public cPacketBase<cEveBreakBlocks, PacketId::EVE_BREAK_BLOCKS>
{
public:
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    std::vector<BlockFormat> mBreakFormats;
};
}
}
}