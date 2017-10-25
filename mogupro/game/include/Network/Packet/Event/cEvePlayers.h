#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <cinder/Vector.h>
#include <cinder/Quaternion.h>
#include <vector>
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
        PlayerFormat( )
        {

        }
        PlayerFormat( cinder::vec3 const& position, cinder::quat const& rotation )
            : mPosition( position )
            , mRotation( rotation )
        {

        }
        cinder::vec3 mPosition;
        cinder::quat mRotation;
    };
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    std::vector<PlayerFormat> mPlayerFormats;
};
}
}
}