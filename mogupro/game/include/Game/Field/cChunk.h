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


    ci::ivec3 getCell();
    std::shared_ptr<cBlock> getBlock( int x, int y, int z );
    std::shared_ptr<cBlock> getBlock( ci::ivec3 c );
    cChunk& getChunk( ci::ivec3 block_cell );

    void setBlock( ci::ivec3 c, std::shared_ptr<cBlock> block );

    void addFace( const std::array<GLfloat, 12>& block_face,
                  const std::array<ci::vec2, 4>& texture_coords,
                  const ci::ivec3 & chunk_position,
                  const ci::vec3 & block_position );

    cChunk& breakBlock( ci::ivec3 c );

    void buildMesh();
    void reBuildStart();
    void reBuildMesh();
    bool reLoading();
    bool createMainCall();
    void createBlocks();

private:

    void clearMesh();
    ci::ivec3 toWorldPosition( ci::ivec3 c )const;
    bool isOutOfRange( ci::ivec3 c );
    int getIndex( ci::ivec3 c );
    int getIndex( int x, int y, int z );

public:

    bool mIsBlockBroken = false;
    bool mHasBuild = false;
    bool mIsLoading = false;
    bool mHasBuildCompleted = false;

    bool mIsLoaded = false;
    bool mIsDone = false;
    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVbo;

private:

    ci::ivec3 mChunkCell;
    uint mIndicesIndex = 0;
    std::array<std::shared_ptr<cBlock>, CHUNK_VOLUME> mBlocks;
    cUnderGround* mUnderGround;
};
}
}
