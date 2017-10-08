#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
#pragma pack(1)
class cResPlayer : public cPacketBase<cResPlayer, PacketId::RES_PLAYER>
{
public:
    void packetImport( ubyte2 size, char const* const data ) override
    {

    }
    ubyte2 packetExport( char* const data ) override
    {
        return 0;
    }
    // サーバーからidが付属されます。
    ubyte1 id;
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