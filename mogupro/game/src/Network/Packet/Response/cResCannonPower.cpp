#include <Network/Packet/Response/cResCannonPower.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Response
{
cResCannonPower::cResCannonPower( )
{

}
void cResCannonPower::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // imp >> pos1 >> a1 >> pos2 >> a2;
	imp >> redTeamPower >> blueTeamPower;
}
ubyte2 cResCannonPower::packetExport( char* const data )
{
    cExporter exp( data );
    // 使い方: vec2 pos1, pos2; ubyte2 a1, a2;
    // exp << pos1 << a1 << pos2 << a2;
	exp << redTeamPower << blueTeamPower;
    return exp;
}
}
}
}