#pragma once
#include <cinder/Noncopyable.h>
#include <Utility/cRecursionUsableMutex.h>
namespace Utility
{
// std::unique_lock<std::mutex>でも代用できるかと思ったのですが、
// lock(); lock(); ができなかったので、使用しないことにしました。
// スコープ内で別スレッドは作成できません。

// だめな例
//scoped_mutex s( /*recursion_usable_mutex*/ mutex );
//std::thread thread( [ ] 
//{
//    scoped_mutex s( /*recursion_usable_mutex*/ mutex );
//} );
//thread.join( );

// 良い例
//{
//    scoped_mutex s( /*recursion_usable_mutex*/ mutex );
//}
//std::thread thread( [ ] 
//{
//    scoped_mutex s( /*recursion_usable_mutex*/ mutex );
//} );
//thread.join( );
class cScopedMutex : cinder::Noncopyable
{
    cRecursionUsableMutex& mMutex;
public:
    cScopedMutex( cRecursionUsableMutex& mutex );
    ~cScopedMutex( );
};
}