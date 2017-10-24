#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckBeginGame : public cPacketBase<cReqCheckBeginGame, PacketId::REQ_CHECK_BEGIN_GAME>
{
public:
    cReqCheckBeginGame( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	cNetworkHandle mNetworkHandle;
};
}
}
}