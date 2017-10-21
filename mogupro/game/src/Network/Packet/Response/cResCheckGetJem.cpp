#include <Network/Packet/Response/cResCheckGetJem.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Response
{
cResCheckGetJem::cResCheckGetJem( )
{

}
cResCheckGetJem::cResCheckGetJem(char flag, short drillID, short gemID) : mFlag(flag),mDrillID(drillID),mGemID(gemID)
{


}
void cResCheckGetJem::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	int offset = 0;
	importChar(mFlag,data,offset,5);
	importShort(mDrillID, data, offset, 5);
	importShort(mGemID, data, offset, 5);
}
ubyte2 cResCheckGetJem::packetExport( char* const data )
{
	int offset = 0;
	exportChar(mFlag, data, offset, 5);
	exportShort(mDrillID, data, offset, 5);
	exportShort(mGemID, data, offset, 5);
    return offset;
}
}
}
}