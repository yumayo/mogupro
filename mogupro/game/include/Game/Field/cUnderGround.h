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

    bool blockBreak( const ci::vec3& position, const float& radius, const cBreakBlockType& type );
    bool blockBreakNetwork( const ci::vec3& position, const float & radius, const cBreakBlockType& type );
    ci::vec3 getBlockTopPosition( const ci::vec3& target_position );
    ci::vec3 getBlockHighestPosition( const ci::vec3& target_position );
    bool isBreakBlock( const vec3& position, const float& raiuds );

public: // Field ì‡Ç≈égÇ§ä÷êî

    cBlock* getBlock( const ci::vec3& position );
    cChunkLayer* getChunkLayer( const ci::vec3& position );

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
