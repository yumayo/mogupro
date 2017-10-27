#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResInRoom : public cPacketBase<cResInRoom, PacketId::RES_IN_ROOM>
{
public:
    cResInRoom( );
	cResInRoom(char flag);
    void packetImport(cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data) override;
    ubyte2 packetExport( char* const data ) override;
	char mFlag;
};
}
}
}