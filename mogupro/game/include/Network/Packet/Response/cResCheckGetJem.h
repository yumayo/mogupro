#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckGetJem : public cPacketBase<cResCheckGetJem, PacketId::RES_CHECK_GET_JEM>
{
public:
    cResCheckGetJem( );
	cResCheckGetJem(char flag,short drillID,short gemID);
	void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;

	char mFlag;
	short mDrillID;
	short mGemID;
};
}
}
}