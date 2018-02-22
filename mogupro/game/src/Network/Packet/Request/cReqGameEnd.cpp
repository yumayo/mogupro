#include <Network/Packet/Request/cReqGameEnd.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqGameEnd::cReqGameEnd( )
{

}
void cReqGameEnd::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    this->networkHandle = networkHandle;
    cImporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet >> pos1 >> a1 >> pos2 >> a2;
}
ubyte2 cReqGameEnd::packetExport( char* const data )
{
    cExporter packet( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // packet << pos1 << a1 << pos2 << a2;
    return packet.getSize( );
}
}
}
}