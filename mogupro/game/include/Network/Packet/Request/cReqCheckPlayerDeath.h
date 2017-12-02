#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckPlayerDeath : public cPacketBase<cReqCheckPlayerDeath, PacketId::REQ_CHECK_PLAYER_DEATH>
{
public:
    cReqCheckPlayerDeath( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	cNetworkHandle networkHandle;
	ubyte1 enemyId;
	ubyte1 playerId;
};
}
}
}