#include <Network/Packet/Request/cReqDamage.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqDamage::cReqDamage( )
{

}
void cReqDamage::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    // Žg‚¢•û: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
	imp >> playerId >> enemyId >> damage;
}
ubyte2 cReqDamage::packetExport( char* const data )
{
    cExporter exp( data );
    // Žg‚¢•û: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
	exp << playerId << enemyId << damage;
    return exp.getSize( );
}
}
}
}