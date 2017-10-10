#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResGetJemPoint : public cPacketBase<cResGetJemPoint, PacketId::RES_GET_JEM_POINT>
{
public:
    cResGetJemPoint( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}