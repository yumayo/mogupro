#include <Network/Packet/_PACKET_TYPE_/_TEMPLATE_CLASS_.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace _PACKET_TYPE_
{
_TEMPLATE_CLASS_::_TEMPLATE_CLASS_( )
{

}
void _TEMPLATE_CLASS_::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    this->networkHandle = networkHandle;
    cImporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet >> pos1 >> a1 >> pos2 >> a2;
}
ubyte2 _TEMPLATE_CLASS_::packetExport( char* const data )
{
    cExporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet << pos1 << a1 << pos2 << a2;
    return packet;
}
}
}
}