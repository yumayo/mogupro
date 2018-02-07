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
class cReqWeaponCapsule : public cPacketBase<cReqWeaponCapsule, PacketId::REQ_WEAPON_CAPSULE>
{
public:
    cReqWeaponCapsule( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
	ci::vec3 position;
	ci::vec3 speed;
	ubyte1 playerId;
	ubyte1 type;
};
}
}
}