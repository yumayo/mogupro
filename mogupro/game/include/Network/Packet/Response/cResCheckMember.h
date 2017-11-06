#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
class cResCheckMember : public cPacketBase<cResCheckMember, PacketId::RES_CHECK_MEMBER>
{
public:
    cResCheckMember( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
	std::string mNameStr;
	std::string mPlayerID;
};
}
}
}