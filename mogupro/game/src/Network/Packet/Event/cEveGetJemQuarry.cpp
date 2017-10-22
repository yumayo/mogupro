#include <Network/Packet/Event/cEveGetJemQuarry.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Event
{
cEveGetJemQuarry::cEveGetJemQuarry( )
{

}
void cEveGetJemQuarry::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
    imp >> mDrillId >> mGemId;
}
ubyte2 cEveGetJemQuarry::packetExport( char* const data )
{
    cExporter exp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
    exp << mDrillId << mGemId;
    return exp;
}
}
}
}