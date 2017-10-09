#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqGetJemPoint : public cPacketBase<cReqGetJemPoint, PacketId::REQ_GET_JEM_POINT>
{
public:
    cReqGetJemPoint( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}