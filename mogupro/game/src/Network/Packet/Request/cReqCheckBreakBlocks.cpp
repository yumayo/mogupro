#include <Network/Packet/Request/cReqCheckBreakBlocks.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckBreakBlocks::cReqCheckBreakBlocks( )
{

}
void cReqCheckBreakBlocks::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    ubyte2 offset = 0;
    for ( int i = 0; i < transferredBytes / ( sizeof( ubyte2 ) * 3 ); ++i )
    {
        float p[3];
        for ( int n = 0; n < 3; ++n )
        {
            ubyte2 src;
            offset = imp( src, data, offset );
            ubyte2 integer = src >> 7;
            ubyte2 decimal = src & 0x007F;
            p[n] = integer;
            p[n] += static_cast<float>( decimal ) / 100.0F;
        }
        mBreakPositions.emplace_back( p[0], p[1], p[2] );
    }
}
ubyte2 cReqCheckBreakBlocks::packetExport( char* const data )
{
    ubyte2 offset = 0;
    for ( auto& o : mBreakPositions )
    {
        ubyte2 p[3];
        for ( int n = 0; n < 3; ++n )
        {
            p[n] = static_cast<ubyte2>( o[n] ) << 7;
            p[n] += static_cast<ubyte2>( ( o[n] - static_cast<int>( o[n] ) ) * 100.0F );
            offset = exp( p[n], data, offset );
        }
    }
    return offset;
}
}
}
}