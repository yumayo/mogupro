#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckLightBomb : public cPacketBase<cReqCheckLightBomb, PacketId::REQ_CHECK_LIGHT_BOMB>
{
public:
    cReqCheckLightBomb( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	ubyte1 playerId;
	cinder::vec3 position;
	cinder::vec3 speed;
};
}
}
}