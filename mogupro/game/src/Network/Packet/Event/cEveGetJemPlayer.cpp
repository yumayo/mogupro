#include <Network/Packet/Event/cEveGetJemPlayer.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Event
{
cEveGetJemPlayer::cEveGetJemPlayer( )
{

}
void cEveGetJemPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    // Žg‚¢•û: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
	imp >> mPlayerId >> mGemId;
}
ubyte2 cEveGetJemPlayer::packetExport( char* const data )
{
    cExporter exp( data );
    // Žg‚¢•û: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
	exp << mPlayerId >> mGemId;
    return exp;
}
}
}
}