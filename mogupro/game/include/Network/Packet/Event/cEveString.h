#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
class cEveString : public cPacketBase<cEveString, PacketId::EVE_STRING>
{
public:
    cEveString( );
    cEveString( std::string const& str );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    std::string str;
};
}
}
}