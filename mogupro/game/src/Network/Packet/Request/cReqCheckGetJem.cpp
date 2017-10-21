#include <Network/Packet/Request/cReqCheckGetJem.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckGetJem::cReqCheckGetJem( )
{

}
cReqCheckGetJem::cReqCheckGetJem(short drillID, short gemID):mDrillID(drillID),mGemID(gemID)
{

}
void cReqCheckGetJem::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	int offset = 0;
	importShort(mDrillID,data,offset,4);
	importShort(mGemID, data, offset, 4);
}
ubyte2 cReqCheckGetJem::packetExport( char* const data )
{
	int offset = 0;
	exportShort(mDrillID, data, offset, 4);
	exportShort(mGemID, data, offset, 4);
	return offset;
}
}
}
}