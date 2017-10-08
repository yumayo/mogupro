#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace Request
{
#pragma pack(1)
class cReqString : public cPacketBase<cReqString, PacketId::REQ_STRING>
{
public:
    cReqString( )
        : str( )
    {

    }
    cReqString( std::string str )
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