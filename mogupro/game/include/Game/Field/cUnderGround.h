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
    void shutdown();

public: // Player,Strategy Ç≈égÇ§ä÷êî

    bool blockBreak( const ci::vec3& position, const float& radius );
    bool blockBreakNetwork( const ci::vec3& position, const float & radius );
    ci::vec3 getBlockTopPosition( const ci::vec3& target_position );
    ci::vec3 getBlockHighestPosition( const ci::vec3& target_position );
    ci::ivec3 getBlockMaxCell();

public: // Field ì‡Ç≈égÇ§ä÷êî

    std::shared_ptr<cBlock> getBlock( const ci::ivec3& position );
    void setBlock( const ci::ivec3& position, const std::shared_ptr<cBlock>& block );
    cChunk& getChunk( const ci::ivec3& position );


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
