#include <Utility/cRecursionUsableMutex.h>
namespace Utility
{
void cRecursionUsableMutex::lock( ) { mMutex.lock( ); }
void cRecursionUsableMutex::unlock( ) { mMutex.unlock( ); }
std::pair<std::map<std::thread::id, int>::iterator, bool> cRecursionUsableMutex::emplace( )
{
    mOwnIds.lock( );
    auto itr = mIds.emplace( std::this_thread::get_id( ), 0 );
    mOwnIds.unlock( );
    return itr;
}
void cRecursionUsableMutex::erase( )
{
    mOwnIds.lock( );
    mIds.erase( std::this_thread::get_id( ) );
    mOwnIds.unlock( );
}
int & cRecursionUsableMutex::getDuplicationCount( )
{
    return mIds.at( std::this_thread::get_id( ) );
}
}
