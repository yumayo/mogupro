#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResSetGamestartTimer : public cPacketBase<cResSetGamestartTimer, PacketId::RES_SET_GAMESTART_TIMER>
{
public:
    cResSetGamestartTimer( );
	cResSetGamestartTimer(std::string timerStr);
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	std::string mTimerStr;

};
}
}
}