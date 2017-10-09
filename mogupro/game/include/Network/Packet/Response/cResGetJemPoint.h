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
class cResGetJemPoint : public cPacketBase<cResGetJemPoint, PacketId::RES_GET_JEM_POINT>
{
public:
    cResGetJemPoint( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
#pragma pack()
}
}
}