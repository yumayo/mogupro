#pragma once

#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>

namespace Network
{
namespace Packet
{
namespace Response
{
class cResWantTeamIn : public cPacketBase<cResWantTeamIn, PacketId::RES_WANT_TEAM_IN>
{
public:
    cResWantTeamIn( );
	cResWantTeamIn(char flag, char teamNum);
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	char mFlag;
	char mTeamNum;
};
}
}
}