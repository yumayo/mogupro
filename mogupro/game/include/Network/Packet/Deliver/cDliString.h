#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
#include <string>
namespace Network
{
namespace Packet
{
namespace Deliver
{
class cDliString : public cPacketBase<cDliString, PacketId::DLI_STRING>
{
public:
    cDliString( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
public:
    std::string str;
};
}
}
}