#include <Network/cRequestManager.h>
namespace Network
{
Packet::Request::cReqString&& cRequestManager::getReqString( )
{
    auto top = mReqString.top( );
    mReqString.pop( );
    return std::move( top );
}
void cRequestManager::ungetReqString( Packet::Request::cReqString const & data )
{
    mReqString.push( data );
}
}