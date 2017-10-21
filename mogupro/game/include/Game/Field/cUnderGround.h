#pragma once
#include <Game/Field/cBlock.h>
#include <Game/Field/cChunk.h>
#include <Game/Field/cChunkHolder.h>
#include <Game/Field/FieldData.h>
#include <thread>
#include <mutex>
#include <atomic>
#include <boost/lockfree/queue.hpp>

namespace Game
{
namespace Field
{
class cUnderGround
{
public:

    cUnderGround();
    ~cUnderGround();

    void setup();
    void update();
    void draw();

public: // Player,Strategy Ç≈égÇ§ä÷êî

    bool blockBreak( const ci::vec3& position, const float& radius );
    ci::vec3 getBlockCenterTopPosition( const ci::vec3& target_position );
    ci::ivec3 getBlockMaxCell();

private: // Method

    bool createChunks( int x, int z );

    ci::ivec3 getChunkCellFromPosition( const ci::vec3& position );
    ci::ivec3 getBlockCellFromPosition( const ci::vec3& position );

private: // Member

    cChunkHolder mChunkHolder;
    std::vector<std::thread> mChunkLoadThreads;
    std::mutex mMainMutex;
    std::atomic<bool> mIsRunning{ true };
    //boost::lockfree::queue<int> mLockFreeQueue;


};
}
}
