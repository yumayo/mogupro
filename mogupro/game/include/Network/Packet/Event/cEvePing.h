#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <string>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEvePing : public cPacketBase<cEvePing, PacketId::EVE_PING>
{
public:
    cEvePing( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    cNetworkHandle mNetworkHandle;
	std::string time;
};
}
}
}