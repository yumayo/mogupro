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

cResWantTeamIn::cResWantTeamIn(char flag, char teamNum) :mFlag(flag),mTeamNum(teamNum)
{
}

void cResWantTeamIn::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	int offset = 0;
	importChar(mFlag,data,offset,2);
	importChar(mTeamNum, data, offset, 2);
}

ubyte2 cResWantTeamIn::packetExport( char* const data )
{
	int offset = 0;
	exportChar(mFlag, data, offset, 2);
	exportChar(mTeamNum, data, offset, 2);
    return offset;
}
}
}
}