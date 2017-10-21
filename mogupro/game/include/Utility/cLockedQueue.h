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
        std::lock_guard<std::mutex> lock( mutexque_ );
        que_.emplace( std::forward<Args>( args )... );
        popwait_.notify_one();
    }

    template<typename ...Args>
    void triPush()
    {

    }

    T pop()
    {
        std::unique_lock<std::mutex> lock( mutexque_ );
        popwait_.wait( lock, [this] { return !que_.empty(); } );
        T data = std::move( que_.front() );
        que_.pop();
        return data;
    }

public:

    std::queue<T> mQue;
    std::mutex mMutexQue;
    std::condition_variable mCondition;
};
}
