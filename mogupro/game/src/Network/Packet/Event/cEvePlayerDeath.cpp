#include <Network/Packet/Event/cEvePlayerDeath.h>
namespace Network
{
namespace Packet
{
namespace Event
{
cEvePlayerDeath::cEvePlayerDeath( )
    : mPlayerName( )
{

}
cEvePlayerDeath::cEvePlayerDeath( std::string const & playerName )
    : mPlayerName( playerName )
{

}
void cEvePlayerDeath::packetImport( cNetworkHandle networkHandle, ubyte2 transferredBytes, char const* const data )
{
    mPlayerName = data;
}
ubyte2 cEvePlayerDeath::packetExport( char* const data )
{
    memcpy( data, mPlayerName.data( ), mPlayerName.size( ) );
    return mPlayerName.size( );
}
}
}
}