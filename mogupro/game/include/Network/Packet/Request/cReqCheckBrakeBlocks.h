#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckBrakeBlocks : public cPacketBase<cReqCheckBrakeBlocks, PacketId::REQ_CHECK_BRAKE_BLOCKS>
{
public:
    cReqCheckBrakeBlocks( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}