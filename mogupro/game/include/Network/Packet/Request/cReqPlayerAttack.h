#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqPlayerAttack : public cPacketBase<cReqPlayerAttack, PacketId::REQ_PLAYER_ATTACK>
{
public:
    cReqPlayerAttack( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
	ubyte1 playerId;
	ubyte1 call;
};
}
}
}