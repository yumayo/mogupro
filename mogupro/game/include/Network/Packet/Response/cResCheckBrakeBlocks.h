#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckBrakeBlocks : public cPacketBase<cResCheckBrakeBlocks, PacketId::RES_CHECK_BRAKE_BLOCKS>
{
public:
    cResCheckBrakeBlocks( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}