#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckGetJemPlayer : public cPacketBase<cResCheckGetJemPlayer, PacketId::RES_CHECK_GET_JEM_PLAYER>
{
public:
    cResCheckGetJemPlayer( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    ubyte1 mIsSuccessed;
    ubyte1 mPlayerId;
    ubyte2 mGemId;
};
}
}
}