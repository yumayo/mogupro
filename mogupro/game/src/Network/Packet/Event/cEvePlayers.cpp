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
    for ( int i = 0; i < transferredBytes / ( sizeof( cinder::vec3 ) + sizeof( cinder::quat ) ); ++i )
    {
        cinder::vec3 position;
        cinder::quat rotation;
        imp >> position >> rotation;
        mPlayerFormats.emplace_back( position, rotation );
    }
}
ubyte2 cEvePlayers::packetExport( char* const data )
{
    cExporter exp( data );
    for ( auto& o : mPlayerFormats )
    {
        exp << o.mPosition << o.mRotation;
    }
    return exp;
}
}
}
}