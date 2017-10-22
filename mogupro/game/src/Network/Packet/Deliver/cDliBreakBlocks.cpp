#include <Network/Packet/Deliver/cDliBreakBlocks.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Deliver
{
cDliBreakBlocks::cDliBreakBlocks( )
{

}
void cDliBreakBlocks::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter imp( data );
    for ( int i = 0; i < transferredBytes / ( sizeof( ubyte2 ) * 3 ); ++i )
    {
        float p[3];
        for ( int n = 0; n < 3; ++n )
        {
            ubyte2 src;
            imp >> src;
            p[n] = convertFloat( src );
        }
        mBreakPositions.emplace_back( p[0], p[1], p[2] );
    }
}
ubyte2 cDliBreakBlocks::packetExport( char* const data )
{
    cExporter exp( data );
    for ( auto& o : mBreakPositions )
    {
        for ( int n = 0; n < 3; ++n )
        {
            exp << convertFixedpoint( o[n] );
        }
    }
    return exp;
}
}
}
}