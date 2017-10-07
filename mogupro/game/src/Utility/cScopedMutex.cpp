#include <Utility/cScopedMutex.h>
namespace Utility
{
cScopedMutex::cScopedMutex( cRecursionUsableMutex & mutex )
    : mMutex( mutex )
{
    auto& itr = mMutex.emplace( );
    if ( itr.second )
    {
        mMutex.lock( );
    }
    else
    {
        ++itr.first->second;
    }
}
cScopedMutex::~cScopedMutex( )
{
    auto& duplication_count = mMutex.getDuplicationCount( );
    if ( duplication_count == 0 )
    {
        mMutex.unlock( );
        mMutex.erase( );
    }
    else
    {
        --duplication_count;
    }
}
}