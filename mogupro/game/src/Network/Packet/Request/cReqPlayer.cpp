#include <Network/Packet/Request/cReqPlayer.h>
#include <Network/PackFunction.hpp>
namespace Network
{
namespace Packet
{
namespace Request
{
void cReqPlayer::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    cImporter( data ) >> mPosition >> mRotation;
}
ubyte2 cReqPlayer::packetExport( char* const data )
{
    return cExporter( data ) << mPosition << mRotation;
}
}
}
}