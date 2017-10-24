#include <Network/Packet/Request/cReqConnect.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqConnect::cReqConnect( )
{

}
void cReqConnect::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    mNetworkHandle = networkHandle;
    cImporter imp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
}
ubyte2 cReqConnect::packetExport( char* const data )
{
    cExporter exp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
    return exp;
}
}
}
}