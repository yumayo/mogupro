#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqEndStartTimer : public cPacketBase<cReqEndStartTimer, PacketId::REQ_END_START_TIMER>
{
public:
    cReqEndStartTimer( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	cNetworkHandle mNetworkHandle;
};
}
}
}