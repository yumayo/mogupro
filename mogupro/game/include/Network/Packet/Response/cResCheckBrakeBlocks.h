#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckBrakeBlocks : public cPacketBase<cResCheckBrakeBlocks, PacketId::RES_CHECK_BREAK_BLOCKS>
{
public:
    cResCheckBrakeBlocks( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	int xPos;
	int yPos;
	int zPos;

};
}
}
}