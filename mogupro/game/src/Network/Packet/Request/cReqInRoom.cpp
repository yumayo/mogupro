#include <Network/Packet/Request/cReqInRoom.h>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqInRoom::cReqInRoom( ) : mRoomID(-1), mNetworkHandle("",0)
{
}
cReqInRoom::cReqInRoom( int roomID) : mRoomID(roomID), mNetworkHandle("", 0)
{
}

void cReqInRoom::packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data)
{	mNetworkHandle = networkHandle;	int offset = 0;	importInt(mRoomID, data, offset, 4);
}
ubyte2 cReqInRoom::packetExport( char* const data )
{	int offset = 0;	exportInt(mRoomID, data, offset, 4);
	return offset;
}
}
}
}