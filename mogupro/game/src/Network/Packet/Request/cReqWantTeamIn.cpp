#include <Network/Packet/Request/cReqWantTeamIn.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqWantTeamIn::cReqWantTeamIn( ) : mNetworkHandle("",0)
{

}
cReqWantTeamIn::cReqWantTeamIn(char teamNum) :mTeamNum(teamNum), mNetworkHandle("", 0)
{

}
void cReqWantTeamIn::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
	mNetworkHandle = networkHandle;
	int offset = 0;
	importChar(mTeamNum, data, offset, 1);
}
ubyte2 cReqWantTeamIn::packetExport( char* const data )
{
	int offset = 0;
	exportChar(mTeamNum,data,offset,1);

    return offset;
}
}
}
}