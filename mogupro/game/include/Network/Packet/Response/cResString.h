#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Response
{
#pragma pack(1)
class cResString : public cPacketBase<cResString, PacketId::RES_STRING>
{
public:
    cResString( )
        : str( )
    {

    }
    cResString( std::string str )
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