#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <vector>
#include <cinder/Vector.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResBreakBlocks : public cPacketBase<cResBreakBlocks, PacketId::RES_BREAK_BLOCKS>
{
public:
    cResBreakBlocks( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    std::vector<cinder::vec3> mBreakPositions;
};
}
}
}