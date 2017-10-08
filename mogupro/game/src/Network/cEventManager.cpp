#include <Network/cEventManager.h>
namespace Network
{
Packet::Event::cEveString&& cEventManager::getEveString( )
{
    auto top = mEveString.top( );
    mEveString.pop( );
    return std::move( top );
}
void cEventManager::ungetEveString( Packet::Event::cEveString const & data )
{
    mEveString.push( data );
}
}