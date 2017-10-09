#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqPlayer : public cPacketBase<cReqPlayer, PacketId::REQ_PLAYER>
{
public:
    cReqPlayer( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    float xPos;
    float yPos;
    float zPos;
    float xDir;
    float yDir;
    float zDir;
};
}
}
}