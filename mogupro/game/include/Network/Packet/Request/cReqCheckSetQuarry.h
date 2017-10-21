#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckSetQuarry : public cPacketBase<cReqCheckSetQuarry, PacketId::REQ_CHECK_SET_QUARRY>
{
public:
    cReqCheckSetQuarry( );
	cReqCheckSetQuarry(float xPos,float yPos,float zPos,char type);
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	float mXPos;
	float mYPos;
	float mZPos;
	char mType;
};
}
}
}