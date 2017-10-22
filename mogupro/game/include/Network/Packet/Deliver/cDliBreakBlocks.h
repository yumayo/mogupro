#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <vector>
#include <cinder/Vector.h>
namespace Network
{
namespace Packet
{
namespace Deliver
{
class cDliBreakBlocks : public cPacketBase<cDliBreakBlocks, PacketId::DLI_BREAK_BLOCKS>
{
public:
    cDliBreakBlocks( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    std::vector<cinder::vec3> mBreakPositions;
};
}
}
}