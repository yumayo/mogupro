#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResEndGamemainSetup : public cPacketBase<cResEndGamemainSetup, PacketId::RES_END_GAMEMAIN_SETUP>
{
public:
    cResEndGamemainSetup( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
}
}
}