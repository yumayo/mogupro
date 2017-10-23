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
    for ( int i = 0; i < transferredBytes / ( sizeof( ubyte1 ) + sizeof( cinder::vec3 ) + sizeof( cinder::quat ) ); ++i )
    {
        ubyte1 id;
        imp >> id;
        auto& m = mPlayerFormats[id];
        imp >> m.mPosition >> m.mRotation;
    }
}
ubyte2 cEvePlayers::packetExport( char* const data )
{
    cExporter exp( data );
    for ( auto& o : mPlayerFormats )
    {
        exp << o.first << o.second.mPosition << o.second.mRotation;
    }
    return exp;
}
}
}
}