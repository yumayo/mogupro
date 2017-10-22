#include <Network/Packet/Response/cResCheckGetJemPlayer.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Response
{
cResCheckGetJemPlayer::cResCheckGetJemPlayer( )
{

}
void cResCheckGetJemPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
    imp >> mIsSuccessed >> mPlayerId >> mGemId;
}
ubyte2 cResCheckGetJemPlayer::packetExport( char* const data )
{
    cExporter exp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
    exp << mIsSuccessed << mPlayerId << mGemId;
    return exp;
}
}
}
}