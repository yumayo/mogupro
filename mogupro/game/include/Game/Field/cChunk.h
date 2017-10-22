#pragma once
#include <Game/Field/cBlock.h>
#include <Game/Field/FieldData.h>

namespace Game
{
namespace Field
{
class cUnderGround;
class cChunk
{
public:

    cChunk();
    cChunk( int x, int z, cUnderGround* under_ground );
    ~cChunk();

    void setup();
    void update();
    void draw();


    ci::ivec3 getCell() { return mChunkCell; }
    cBlock& getBlock( int x, int y, int z );
    cBlock& getBlock( ci::ivec3 c );
    std::array<cBlock, CHUNK_VOLUME>& getBlocks();

    void setBlock( ci::ivec3 c, cBlock block );

    void addFace( const std::array<GLfloat, 12>& block_face,
                  const std::array<ci::vec2, 4>& texture_coords,
                  const ci::ivec3 & chunk_position,
                  const ci::vec3 & block_position );

    void breakBlock( ci::ivec3 c );


    bool createMainCall();
    void createBlocks();

private:

    ci::ivec3 toWorldPosition( ci::ivec3 c )const;
    bool isOutOfRange( ci::ivec3 c );
    int getIndex( ci::ivec3 c );
    int getIndex( int x, int y, int z );

public:

    bool mIsLoaded = 0;
    bool mIsDone = 0;
    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVbo;

private:

    ci::ivec3 mChunkCell;
    uint mIndicesIndex = 0;
    std::array<cBlock, CHUNK_VOLUME> mBlocks;
    cUnderGround* mUnderGround;
};
}
}
