#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqCannonPower : public cPacketBase<cReqCannonPower, PacketId::REQ_CANNON_POWER>
{
public:
    cReqCannonPower( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	cNetworkHandle networkHandle;
};
}
}
}