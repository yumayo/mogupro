#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <vector>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckBreakBlocks : public cPacketBase<cReqCheckBreakBlocks, PacketId::REQ_CHECK_BREAK_BLOCKS>
{
public:
    cReqCheckBreakBlocks( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    std::vector<cinder::vec3> mBreakPositions;
};
}
}
}
