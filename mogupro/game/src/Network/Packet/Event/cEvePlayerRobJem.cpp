#include <Network/Packet/Event/cEvePlayerRobJem.h>
namespace Network
{
namespace Packet
{
namespace Event
{
cEvePlayerRobJem::cEvePlayerRobJem( )
    : mPlayerName( )
{

}
cEvePlayerRobJem::cEvePlayerRobJem( std::string const & playerName )
    : mPlayerName( playerName )
{

}
void cEvePlayerRobJem::packetImport( ubyte2 size, char const* const data )
{
    mPlayerName = data;
}
ubyte2 cEvePlayerRobJem::packetExport( char* const data )
{
    memcpy( data, mPlayerName.data( ), mPlayerName.size( ) );
    return mPlayerName.size( );
}
}
}
}