#pragma once
#include <Game/Field/cBlock.h>
#include <Game/Field/cChunkLayer.h>
#include <Game/Field/FieldData.h>

namespace Game
{
namespace Field
{
class cUnderGround;
class cChunk : public std::enable_shared_from_this<cChunk>
{
public:

    cChunk();
    cChunk( const int& x,
            const int& z,
            const int& id,
            cUnderGround* under_ground );
    ~cChunk();

    void setup();
    void update( const float& delta_time );
    void draw();

public:

    ci::ivec3 getCell();
    cBlock* getBlock( const int& x, const int& y, const int& z );
    cBlock* getBlock( const ci::ivec3& cell );
    void setBlock( const ci::ivec3& cell, cBlock* block );
    cChunkLayer* getChunkLayer( const int& height );

    void buildMesh();
    void reBuildStart();
    void reBuildMesh();
    bool reLoading();
    bool createMainCall();
    void createBlocks();

private:

    bool isOutOfRange( ci::ivec3 c );

private:

    ci::ivec3 mChunkCell;
    std::vector<cChunkLayer> mChunkLayers;
    cUnderGround* mUnderGround;
    int mChunkId;

};
}
}
