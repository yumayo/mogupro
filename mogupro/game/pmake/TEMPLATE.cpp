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
void _TEMPLATE_CLASS_::packetImport( ubyte2 size, char const* const data )
{
    // dataからクラスを再構成してください。
}
ubyte2 _TEMPLATE_CLASS_::packetExport( char* const data )
{
    // dataにこのクラスの情報を詰めてください。
    return 0 /*パケットサイズを返してください。*/;
}
}
}
}