#pragma once
#include <Game/Field/FieldData.h>

namespace Game
{
namespace Field
{
class cUnderGround;
class cChunkLayer
{
public:

    cChunkLayer();
    cChunkLayer( const int& height, cChunk* chunk, cUnderGround* under_ground );
    ~cChunkLayer();

    void setup();
    void draw();

public:

    cBlock* getBlock( const ci::ivec3& c );
    cBlock* getBlock( const int& x, const  int& y, const int& z );
    cChunk* getChunk( const ci::ivec3& block_cell );
    cChunkLayer* getChunkLayer( const int& height );

    int getHeight() { return mHeight; }
    ci::ivec3 getChunkCell();

    void addFace( const std::array<GLfloat, 12>& block_face,
                  const std::array<ci::vec2, 4>& texture_coords,
                  const ci::vec3 & block_position );
public:

    cChunkLayer* breakBlock( ci::ivec3 c );
    void buildMesh();
    void reBuildStart();
    void reBuildMesh();
    bool reLoading();
    bool createMainCall();
    void createBlocks();

public:

    void clearMesh();
    ci::ivec3 toWorldPosition( ci::ivec3 c );
    bool outOfBounds( const int& v )const;
    int getIndex( const  ci::ivec3& c );
    int getIndex( const int& x, const  int& y, const int& z );

public:

    bool mIsBlockBroken = false;
    bool mHasBuild = false;
    bool mIsLoading = false;
    bool mHasBuildCompleted = false;
    bool mIsLoaded = false;
    bool mIsDone = false;

private:

    uint mIndicesIndex = 0;

    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVbo;
    std::array<cBlockRef, CHUNK_VOLUME > mBlocks;
    int mHeight;

private:

    cChunk* mChunk;
    cUnderGround * mUnderGround;

};
}
}
