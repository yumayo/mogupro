#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckBeginGame : public cPacketBase<cResCheckBeginGame, PacketId::RES_CHECK_BEGIN_GAME>
{
public:
    cResCheckBeginGame( );
	cResCheckBeginGame(ubyte1 playerID);
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	ubyte1 mPlayerID;
};
}
}
}