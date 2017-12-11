#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqEndGamemainSetup : public cPacketBase<cReqEndGamemainSetup, PacketId::REQ_END_GAMEMAIN_SETUP>
{
public:
    cReqEndGamemainSetup( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	cNetworkHandle mNetworkHandle;
};
}
}
}