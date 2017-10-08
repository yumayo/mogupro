#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
#pragma pack(1)
class cReqPlayer : public cPacketBase<cReqPlayer, PacketId::REQ_PLAYER>
{
public:
    void packetImport( ubyte2 size, char const* const data ) override
    {

    }
    ubyte2 packetExport( char* const data ) override
    {
        return 0;
    }
    float xPos;
    float yPos;
    float zPos;
    float xDir;
    float yDir;
    float zDir;
};
#pragma pack()
}
}
}