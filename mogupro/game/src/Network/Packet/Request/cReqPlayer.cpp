#include <Network/Packet/Request/cReqPlayer.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqPlayer::cReqPlayer( )
{

}
void cReqPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    this->networkHandle = networkHandle;
    cImporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet >> pos1 >> a1 >> pos2 >> a2;
	packet >> mFormat.playerId >> mFormat.position >> mFormat.rotation;
}
ubyte2 cReqPlayer::packetExport( char* const data )
{
    cExporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet << pos1 << a1 << pos2 << a2;
	packet << mFormat.playerId << mFormat.position << mFormat.rotation;
    return packet.getSize( );
}
}
}
}