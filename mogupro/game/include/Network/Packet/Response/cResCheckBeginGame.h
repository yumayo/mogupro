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
class cResCheckBeginGame : public cPacketBase<cResCheckBeginGame, PacketId::RES_CHECK_BEGIN_GAME>
{
public:
    cResCheckBeginGame( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
#pragma pack()
}
}
}