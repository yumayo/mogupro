#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Deliver
{
class cDliPing : public cPacketBase<cDliPing, PacketId::DLI_PING>
{
public:
    cDliPing( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    cNetworkHandle mNetworkHandle;
};
}
}
}