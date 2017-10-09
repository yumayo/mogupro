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
    cResString( );
    cResString( std::string str );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    std::string str;
};
#pragma pack()
}
}
}