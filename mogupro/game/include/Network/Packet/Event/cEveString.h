#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Event
{
#pragma pack(1)
class cEveString : public cPacketBase<cEveString, PacketId::EVE_STRING>
{
public:
    cEveString( )
        : str( )
    {

    }
    cEveString( std::string const& str )
        : str( str )
    {

    }
    void packetImport( ubyte2 size, char const* const data ) override
    {
        str = data;
    }
    ubyte2 packetExport( char* const data ) override
    {
        memcpy( data, str.data( ), str.size( ) );
        return str.size( );
    }
    std::string str;
};
#pragma pack()
}
}
}