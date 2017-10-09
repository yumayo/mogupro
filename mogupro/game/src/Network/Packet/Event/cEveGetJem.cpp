#include <Network/Packet/Event/cEveGetJem.h>
namespace Network
{
namespace Packet
{
namespace Event
{
cEveGetJem::cEveGetJem( )
    : mGemId( )
{

}
cEveGetJem::cEveGetJem( ubyte2 gemId )
    : mGemId( gemId )
{

}
void cEveGetJem::packetImport( ubyte2 size, char const* const data )
{
    memcpy( &mGemId, data, sizeof( mGemId ) );
}
ubyte2 cEveGetJem::packetExport( char* const data )
{
    memcpy( data, &mGemId, sizeof( mGemId ) );
    return sizeof( mGemId );
}
}
}
}