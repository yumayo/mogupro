#include <Network/Packet/Request/cReqInRoomWatching.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqInRoomWatching::cReqInRoomWatching( )
{

}
cReqInRoomWatching::cReqInRoomWatching( byte4 roomID )
	: roomID(roomID)
{
}
void cReqInRoomWatching::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    this->networkHandle = networkHandle;
    cImporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet >> pos1 >> a1 >> pos2 >> a2;
	packet >> roomID;
}
ubyte2 cReqInRoomWatching::packetExport( char* const data )
{
    cExporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet << pos1 << a1 << pos2 << a2;
	packet << roomID;
    return packet.getSize( );
}
}
}
}