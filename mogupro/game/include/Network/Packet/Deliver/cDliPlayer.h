#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
namespace Network
{
namespace Packet
{
namespace Deliver
{
class cDliPlayer : public cPacketBase<cDliPlayer, PacketId::DLI_PLAYER>
{
public:
    cDliPlayer( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    cNetworkHandle mNetworkHandle;
    cinder::vec3 mPosition;
    cinder::quat mRotation;
};
}
}
}