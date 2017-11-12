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
    void update();
    void draw();

public:

    cBlock* getBlock( const int& x, const  int& y, const int& z );
    cBlock* getBlock( const ci::ivec3& c );
    void setBlock( const int& x, const  int& y, const int& z, cBlock* block );
    void setBlock( const ci::ivec3& c, cBlock* block );
    cChunkLayer* getChunkLayer( const int& height );
    cChunkLayer* getChunkLayer( const ci::ivec3& cell );

    int getHeight() { return mHeight; }
    ci::ivec3 getChunkCell();

    void addFace( const std::array<GLfloat, 12>& block_face,
                  const std::array<GLfloat, 12>& block_normal,
                  const std::array<ci::vec2, 4>& texture_coords,
                  const ci::vec3 & block_position );
public:

    cChunkLayer* breakBlock( ci::ivec3 c );
    cChunkLayer* breakBlock( cBlock* block, cChunkLayer* layer );

    void buildMesh();
    void reBuildStart();
    void reBuildMesh();
    bool reLoading();
    bool createMainCall();
    void createBlocks();

public:

    void clearMesh();
    // もらったセルをワールド座標に変換する
    ci::vec3 toWorldPosition( ci::ivec3 c );
    bool outOfBounds( const int& v )const;
    int getIndex( const  ci::ivec3& c );
    int getIndex( const int& x, const  int& y, const int& z );

public: // スレッド用フラグ

    bool mIsRebuildMesh = false;
    bool mHasBuild = false;
    bool mIsLoading = false;
    bool mHasBuildCompleted = false;
    bool mIsLoaded = false;
    bool mIsDone = false;

public:

    bool mIsActive;

private:

    uint mIndicesIndex = 0;

    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVbo;
    std::array<cBlockRef, CHUNK_VOLUME > mBlocks;
    int mHeight;
    int mRevivalTime;
    std::unordered_map<int, float> mRevivalBlocks;

private:

    cChunk *mChunk;
    cUnderGround *mUnderGround;

};
}
}
