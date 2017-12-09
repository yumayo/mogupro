#include <Network/Packet/Event/cEvePlayers.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Event
{
void cEvePlayers::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    for ( int i = 0; i < transferredBytes / ( sizeof( cinder::vec3 ) + sizeof( cinder::vec2 ) ); ++i )
    {
        ubyte1 playerId;
        cinder::vec3 position;
        cinder::vec2 rotation;
        imp >> playerId >> position >> rotation;
        mPlayerFormats.emplace_back( playerId, position, rotation );
    }
}
ubyte2 cEvePlayers::packetExport( char* const data )
{
    cExporter exp( data );
    for ( auto& o : mPlayerFormats )
    {
        exp << o.playerId << o.position << o.rotation;
    }
    return exp;
}
}
}
}