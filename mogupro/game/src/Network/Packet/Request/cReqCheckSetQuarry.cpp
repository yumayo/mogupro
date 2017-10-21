#include <Network/Packet/Request/cReqCheckSetQuarry.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckSetQuarry::cReqCheckSetQuarry( ) 
{

}

cReqCheckSetQuarry::cReqCheckSetQuarry(float xPos, float yPos, float zPos, char type) :
	mXPos(xPos),mYPos(yPos),mZPos(zPos),mType(type)
{


}
void cReqCheckSetQuarry::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	int offset = 0;
	importFloat(mXPos,data,offset,13);
	importFloat(mYPos, data, offset, 13);
	importFloat(mZPos, data, offset, 13);
	importChar(mType, data, offset, 13);

}
ubyte2 cReqCheckSetQuarry::packetExport( char* const data )
{
	int offset = 0;
	exportFloat(mXPos, data, offset, 13);
	exportFloat(mYPos, data, offset, 13);
	exportFloat(mZPos, data, offset, 13);
	exportChar(mType, data, offset, 13);

    return offset;
}
}
}
}