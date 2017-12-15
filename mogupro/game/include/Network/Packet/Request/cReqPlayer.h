#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <Network/Packet/StructureDefines.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqPlayer : public cPacketBase<cReqPlayer, PacketId::REQ_PLAYER>
{
public:
    cReqPlayer( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
	PlayerFormat mFormat;
};
}
}
}