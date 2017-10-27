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
    bool blockBreakNetwork( const ci::vec3& position, const float & radius );
    ci::vec3 getBlockTopPosition( const ci::vec3& target_position );
    ci::vec3 getBlockHighestPosition( const ci::vec3& target_position );
    ci::ivec3 getBlockMaxCell();

public: // Field ì‡Ç≈égÇ§ä÷êî

    cBlock getBlock( ci::ivec3 position );
    void setBlock( ci::ivec3 position, cBlock block );


private: // Method

    bool chunkMeshReLoaded();
    bool calcChunks();

    ci::ivec3 getChunkCellFromPosition( const ci::vec3& position );
    ci::ivec3 getBlockCellFromPosition( const ci::vec3& position );

private: // Member

    cChunkHolder mChunkHolder;
    std::vector<std::thread> mChunkLoadThreads;
    std::mutex mMainMutex;
    std::atomic<bool> mIsRunning{ true };
    std::function<void()> mCallBack = nullptr;

};
}
}
