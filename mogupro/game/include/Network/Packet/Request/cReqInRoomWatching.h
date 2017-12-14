#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqInRoomWatching : public cPacketBase<cReqInRoomWatching, PacketId::REQ_IN_ROOM_WATCHING>
{
public:
	cReqInRoomWatching( );
	cReqInRoomWatching( byte4 roomID );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
	byte4 roomID;
};
}
}
}