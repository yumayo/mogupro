#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqAddCannonPower : public cPacketBase<cReqAddCannonPower, PacketId::REQ_ADD_CANNON_POWER>
{
public:
    cReqAddCannonPower( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	cNetworkHandle networkHandle;
	ubyte1 teamId;
	ubyte1 playerId;
	ubyte1 power;
};
}
}
}