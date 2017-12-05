#pragma once
#include <Network/Packet/cPacketBase.h>
#include <Network/Packet/PacketId.h>
namespace Network
{
namespace Packet
{
namespace _PACKET_TYPE_
{
class _TEMPLATE_CLASS_ : public cPacketBase<_TEMPLATE_CLASS_, PacketId::_TEMPLATE_ENUM_>
{
public:
    _TEMPLATE_CLASS_( );
    void packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data ) override;
    ubyte2 packetExport( char* const data ) override;
    cNetworkHandle networkHandle;
};
}
}
}