#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
#include <map>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEvePlayers : public cPacketBase<cEvePlayers, PacketId::EVE_PLAYERS>
{
public:
    struct PlayerFormat
    {
        cinder::vec3 mPosition;
        cinder::quat mRotation;
    };
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    std::map<ubyte1, PlayerFormat> mPlayerFormats;
};
}
}
}