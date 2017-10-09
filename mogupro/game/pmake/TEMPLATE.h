#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace _PACKET_TYPE_
{
#pragma pack(1)
class _TEMPLATE_CLASS_ : public cPacketBase<_TEMPLATE_CLASS_, PacketId::_TEMPLATE_ENUM_>
{
public:
    _TEMPLATE_CLASS_( );
    void packetImport( ubyte2 size, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
};
#pragma pack()
}
}
}