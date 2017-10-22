#include <Network/Packet/Deliver/cDliString.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Deliver
{
cDliString::cDliString( )
{

}
void cDliString::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
    imp >> str;
}
ubyte2 cDliString::packetExport( char* const data )
{
    cExporter exp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
    exp << str;
    return exp;
}
}
}
}