#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqPlayer : public cPacketBase<cReqPlayer, PacketId::REQ_PLAYER>
{
public:
    cReqPlayer( ) = default;
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cinder::vec3 mPosition;
    cinder::quat mRotation;
};
}
}
}