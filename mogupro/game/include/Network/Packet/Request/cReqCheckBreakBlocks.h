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
class cReqCheckBreakBlocks : public cPacketBase<cReqCheckBreakBlocks, PacketId::REQ_CHECK_BREAK_BLOCKS>
{
public:
    cReqCheckBreakBlocks( );
    cReqCheckBreakBlocks( cinder::ivec3 cellNum );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cinder::ivec3 mCellNum;
};
}
}
}
