#include <Network/cResponseManager.h>
namespace Network
{
// P=====BEGIN=====P
Packet::Response::cResString&& cResponseManager::getResString( )
{
    auto top = mResString.top( );
    mResString.pop( );
    return std::move( top );
}
void cResponseManager::ungetResString( Packet::Response::cResString const & data )
{
    mResString.push( data );
}
// P=====END=====P
}