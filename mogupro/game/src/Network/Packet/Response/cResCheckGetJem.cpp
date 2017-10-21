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
void cResCheckGetJem::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	int offset = 0;
	importChar(mFlag, data, offset, 5);
	importShort(mDrillId, data, offset, 5);
	importShort(mGemId, data, offset, 5);

}
ubyte2 cResCheckGetJem::packetExport( char* const data )
{
	int offset = 0;
	importChar(mFlag, data, offset, 5);
	importShort(mDrillId, data, offset, 5);
	importShort(mGemId, data, offset, 5);
	return offset;
}
}
}
}