#include <Network/Packet/Deliver/cDliPlayer.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Deliver
{
cDliPlayer::cDliPlayer( )
{

}
void cDliPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    mNetworkHandle = networkHandle;
    cImporter imp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
    imp >> mPosition >> mRotation;
}
ubyte2 cDliPlayer::packetExport( char* const data )
{
    cExporter exp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
    exp << mPosition << mRotation;
    return exp;
}
}
}
}