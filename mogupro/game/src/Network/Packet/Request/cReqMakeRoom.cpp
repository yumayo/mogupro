#include <Network/Packet/Request/cReqMakeRoom.h>#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqMakeRoom::cReqMakeRoom( ) : mRoomID(-1), mNetworkHandle("", 0)
{
}cReqMakeRoom::cReqMakeRoom(int roomID) : mRoomID(roomID), mNetworkHandle("", 0)
{

}

void cReqMakeRoom::packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data)
{	mNetworkHandle = networkHandle;	int offset = 0;	importInt(mRoomID,data,offset,4);
}
ubyte2 cReqMakeRoom::packetExport( char* const data )
{	int offset = 0;	exportInt(mRoomID, data, offset, 4);
    return offset;
}
}
}
}