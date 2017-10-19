#include <Network/Packet/Request/cReqCheckBreakBlocks.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
cReqCheckBreakBlocks::cReqCheckBreakBlocks( )
    : mCellNum( cinder::ivec3( 0 ) )
{
}
cReqCheckBreakBlocks::cReqCheckBreakBlocks( cinder::ivec3 cellNum )
    : mCellNum( cellNum )
{

}
void cReqCheckBreakBlocks::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    int offset = 0;
    importInt( mCellNum.x, data, offset, sizeof( cinder::ivec3 ) );
    importInt( mCellNum.y, data, offset, sizeof( cinder::ivec3 ) );
    importInt( mCellNum.z, data, offset, sizeof( cinder::ivec3 ) );
}
ubyte2 cReqCheckBreakBlocks::packetExport( char* const data )
{
    int offset = 0;
    exportInt( mCellNum.x, data, offset, sizeof( cinder::ivec3 ) );
    exportInt( mCellNum.y, data, offset, sizeof( cinder::ivec3 ) );
    exportInt( mCellNum.z, data, offset, sizeof( cinder::ivec3 ) );
    return sizeof( cinder::ivec3 );
}
}
}
}