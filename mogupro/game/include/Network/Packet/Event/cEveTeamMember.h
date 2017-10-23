#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveTeamMember : public cPacketBase<cEveTeamMember, PacketId::EVE_TEAM_MEMBER>
{
public:
    cEveTeamMember( );
	cEveTeamMember(ubyte1 teamNum, std::string nameStr,ubyte1 playerID);
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	ubyte1 mTeamNum;
	std::string mNameStr;
	ubyte1 mPlayerID;
};
}
}
}