#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <map>
#include <cinder/Vector.h>
#include <tuple>
#include <cinder/Quaternion.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResPlayer : public cPacketBase<cResPlayer, PacketId::RES_PLAYER>
{
    struct PlayerFormat
    {
        cinder::vec3 mPosition;
        cinder::quat mRotation;
    };
public:
    cResPlayer( ) = default;
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    std::map<ubyte1, PlayerFormat> mPlayerFormats;
};
}
}
}