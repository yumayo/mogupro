#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <Network/Packet/StructureDefines.h>
#include <vector>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqBreakBlocks : public cPacketBase<cReqBreakBlocks, PacketId::REQ_BREAK_BLOCKS>
{
public:
    cReqBreakBlocks( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
	std::vector<BlockFormat> mBreakFormats;
};
}
}
}