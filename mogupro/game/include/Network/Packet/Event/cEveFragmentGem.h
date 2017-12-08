#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveFragmentGem : public cPacketBase<cEveFragmentGem, PacketId::EVE_FRAGMENT_GEM>
{
public:
    cEveFragmentGem( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
	ubyte1 gemId;
	ubyte4 seed;
};
}
}
}