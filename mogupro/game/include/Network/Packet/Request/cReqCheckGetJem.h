#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>

namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCheckGetJem : public cPacketBase<cReqCheckGetJem, PacketId::REQ_CHECK_GET_JEM>
{
public:
    cReqCheckGetJem( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
	cNetworkHandle handle;
	short mDrillId;
	short mGemId;
};
}
}
}