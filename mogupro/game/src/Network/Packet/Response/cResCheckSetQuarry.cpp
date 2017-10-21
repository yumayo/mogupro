#include <Network/Packet/Response/cResCheckSetQuarry.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Response
{
cResCheckSetQuarry::cResCheckSetQuarry( )
{

}
char mFlag;
float mXPos;
float mYPos;
float mZPos;
char mType;
short mStorageID;

cResCheckSetQuarry::cResCheckSetQuarry(char flag, float xPos, float yPos, float zPos, char type, short storageID) :
	mFlag(flag), mXPos(xPos), mYPos(yPos), mZPos(zPos), mType(type), mStorageID(storageID)
{

}
void cResCheckSetQuarry::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	int offset = 0;
	importChar(mFlag,data,offset,16);
	importFloat(mXPos,data,offset,16);
	importFloat(mYPos, data, offset, 16);
	importFloat(mZPos, data, offset, 16);
	importChar(mType, data, offset, 16);
	importShort(mStorageID,data,offset,16);
}
ubyte2 cResCheckSetQuarry::packetExport( char* const data )
{
	int offset = 0;
	exportChar(mFlag, data, offset, 16);
	exportFloat(mXPos, data, offset, 16);
	exportFloat(mYPos, data, offset, 16);
	exportFloat(mZPos, data, offset, 16);
	exportChar(mType, data, offset, 16);
	exportShort(mStorageID, data, offset, 16);
    return offset;
}
}
}
}