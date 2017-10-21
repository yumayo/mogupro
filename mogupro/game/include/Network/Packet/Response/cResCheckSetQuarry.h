#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckSetQuarry : public cPacketBase<cResCheckSetQuarry, PacketId::RES_CHECK_SET_QUARRY>
{
public:
    cResCheckSetQuarry( );
	cResCheckSetQuarry(char flag,float xPos, float yPos, float zPos,char type,short storageID);
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;

	char mFlag;
	float mXPos; 
	float mYPos; 
	float mZPos; 
	char mType;
	short mStorageID;

};
}
}
}