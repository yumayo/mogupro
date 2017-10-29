#include <Network/Packet/Event/cEveBreakBlocks.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Event
{
void cEveBreakBlocks::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    for ( int i = 0; i < transferredBytes / ( sizeof( ubyte2 ) * 4 ); ++i )
    {
        float p[3];
        for ( int n = 0; n < 3; ++n )
        {
            ubyte2 src;
            imp >> src;
            p[n] = convertFloat( src );
        }
        ubyte2 radius;
        imp >> radius;
        mBreakFormats.emplace_back( cinder::vec3( p[0], p[1], p[2] ), convertFloat( radius ) );
    }
}
ubyte2 cEveBreakBlocks::packetExport( char* const data )
{
    cExporter exp( data );
    for ( auto& o : mBreakFormats )
    {
        for ( int n = 0; n < 3; ++n )
        {
            exp << convertFixedpoint( o.position[n] );
        }
        exp << convertFixedpoint( o.radius );
    }
    return exp;
}
}
}
}