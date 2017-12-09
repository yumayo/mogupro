#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqFragmentGem : public cPacketBase<cReqFragmentGem, PacketId::REQ_FRAGMENT_GEM>
{
public:
    cReqFragmentGem( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
};
}
}
}