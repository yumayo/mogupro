#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
class cReqString : public cPacketBase<cReqString, PacketId::REQ_STRING>
{
public:
    cReqString( );
    cReqString( std::string str );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    std::string str;
};
}
}
}