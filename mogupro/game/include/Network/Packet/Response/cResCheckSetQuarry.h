#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckSetQuarry : public cPacketBase<cResCheckSetQuarry, PacketId::RES_CHECK_SET_QUARRY>
{
public:
    cResCheckSetQuarry( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}