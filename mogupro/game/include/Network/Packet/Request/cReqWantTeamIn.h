#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqWantTeamIn : public cPacketBase<cReqWantTeamIn, PacketId::REQ_WANT_TEAM_IN>
{
public:
    cReqWantTeamIn( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	short mTeamNum;
};
}
}
}