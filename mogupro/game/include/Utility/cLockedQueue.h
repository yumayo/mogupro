#pragma once

#include <queue>
#include <mutex>

namespace Utility
{
template<typename T>
class cLockedQueue
{
public:
    cLockedQueue() {}
    ~cLockedQueue() {}

    template<typename ...Args>
    void push( Args&& ...args )
    {
        std::lock_guard<std::mutex> lock( mMutexQue );
        mQue.emplace( std::forward<Args>( args )... );
        mPopWait.notify_one();
    }

    T pop()
    {
        std::unique_lock<std::mutex> lock( mMutexQue );
        mPopWait.wait( lock, [this]
        {
            return !mQue.empty();
        } );
        T data = std::move( mQue.front() );
        mQue.pop();
        return data;
    }

public:

    std::queue<T> mQue;
    std::mutex mMutexQue;
    std::condition_variable mPopWait;
};
}
