#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveGetJemPlayer : public cPacketBase<cEveGetJemPlayer, PacketId::EVE_GET_JEM_PLAYER>
{
public:
    cEveGetJemPlayer( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    ubyte1 mPlayerId;
    ubyte2 mGemId;
};
}
}
}