#include <Network/Packet/Response/cResPlayer.h>
namespace Network
{
namespace Packet
{
namespace Response
{
cResPlayer::cResPlayer( ubyte1 id, float xPos, float yPos, float zPos, float xTheta, float yTheta )
    : id( id )
    , xPos( xPos )
    , yPos( yPos )
    , zPos( zPos )
    , xTheta( xTheta )
    , yTheta( yTheta )
{

}
void cResPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    size_t offset = 0;
    memcpy( &id, data + offset, sizeof( id ) ); offset += sizeof( id );
    memcpy( &xPos, data + offset, sizeof( xPos ) ); offset += sizeof( xPos );
    memcpy( &yPos, data + offset, sizeof( yPos ) ); offset += sizeof( yPos );
    memcpy( &zPos, data + offset, sizeof( zPos ) ); offset += sizeof( zPos );
    memcpy( &xTheta, data + offset, sizeof( xTheta ) ); offset += sizeof( xTheta );
    memcpy( &yTheta, data + offset, sizeof( yTheta ) ); offset += sizeof( yTheta );
}
ubyte2 cResPlayer::packetExport( char* const data )
{
    size_t offset = 0;
    memcpy( data + offset, &id, sizeof( id ) ); offset += sizeof( id );
    memcpy( data + offset, &xPos, sizeof( xPos ) ); offset += sizeof( xPos );
    memcpy( data + offset, &yPos, sizeof( yPos ) ); offset += sizeof( yPos );
    memcpy( data + offset, &zPos, sizeof( zPos ) ); offset += sizeof( zPos );
    memcpy( data + offset, &xTheta, sizeof( xTheta ) ); offset += sizeof( xTheta );
    memcpy( data + offset, &yTheta, sizeof( yTheta ) ); offset += sizeof( yTheta );
    return offset;
}
}
}
}