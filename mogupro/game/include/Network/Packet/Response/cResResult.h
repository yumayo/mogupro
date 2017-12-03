#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResResult : public cPacketBase<cResResult, PacketId::RES_RESULT>
{
public:
    cResResult( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	ubyte1 redTeamPower;
	ubyte1 blueTeamPower;
};
}
}
}