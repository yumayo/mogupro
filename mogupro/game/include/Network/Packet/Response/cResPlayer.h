#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResPlayer : public cPacketBase<cResPlayer, PacketId::RES_PLAYER>
{
public:
    cResPlayer( ) = default;
    cResPlayer( ubyte1 id, float xPos, float yPos, float zPos, float xTheta, float yTheta );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    // サーバーからidが付属されます。
    ubyte1 id;
    float xPos;
    float yPos;
    float zPos;
	float xTheta;
	float yTheta;
};
}
}
}