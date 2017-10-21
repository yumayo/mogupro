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
void cReqCheckGetJem::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	int offset = 0;
	importShort(mDrillId, data, offset, 4);
	importShort(mGemId, data, offset, 4);
	handle = networkHandle;
}
ubyte2 cReqCheckGetJem::packetExport( char* const data )
{
	int offset = 0;
	importShort(mDrillId, data, offset, 4);
	importShort(mGemId, data, offset, 4);
	return offset;
}
}
}
}