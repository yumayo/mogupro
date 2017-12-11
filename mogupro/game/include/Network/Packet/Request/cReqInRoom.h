#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqInRoom : public cPacketBase<cReqInRoom, PacketId::REQ_IN_ROOM>
{
public:

    cReqInRoom( );

	cReqInRoom(int roomID);

    void packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data) override;

    ubyte2 packetExport( char* const data ) override;

	int mRoomID;
	cNetworkHandle mNetworkHandle;
};
}
}
}