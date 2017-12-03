#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCannonPower : public cPacketBase<cResCannonPower, PacketId::RES_CANNON_POWER>
{
public:
    cResCannonPower( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	ubyte1 redTeamPower;
	ubyte1 blueTeamPower;
};
}
}
}