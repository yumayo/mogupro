#include <Network/Packet/Response/cResWantTeamIn.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Response
{
cResWantTeamIn::cResWantTeamIn( )
{
}

cResWantTeamIn::cResWantTeamIn(char flag, char teamNum,char playerID) : mFlag(flag),mTeamNum(teamNum),mPlayerID(playerID)
{

}

void cResWantTeamIn::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	int offset = 0;
	importChar(mFlag,data,offset,3);
	importChar(mTeamNum, data, offset, 3);
	importChar(mPlayerID, data, offset, 3);
}

ubyte2 cResWantTeamIn::packetExport( char* const data )
{
	int offset = 0;
	exportChar(mFlag, data, offset, 3);
	exportChar(mTeamNum, data, offset, 3);
	exportChar(mPlayerID, data, offset, 3);
    return offset;
}
}
}
}