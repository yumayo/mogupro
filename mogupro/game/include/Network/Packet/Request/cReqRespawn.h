#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqRespawn : public cPacketBase<cReqRespawn, PacketId::REQ_RESPAWN>
{
public:
    cReqRespawn( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	cNetworkHandle networkHandle;
	ubyte1 playerId;
};
}
}
}