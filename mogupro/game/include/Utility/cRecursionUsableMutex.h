#pragma once
#include <mutex>
#include <map>
#include <thread>
namespace Utility
{
class cRecursionUsableMutex
{
    std::mutex mMutex;
    std::mutex mOwnIds;
    std::map<std::thread::id, int /*スレッドの番号の重複回数*/> mIds;
public:
    void lock( );
    void unlock( );
    std::pair<std::map<std::thread::id, int>::iterator, bool> emplace( );
    void erase( );
    int& getDuplicationCount( );
};
}