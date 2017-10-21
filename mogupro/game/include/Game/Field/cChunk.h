#pragma once
#include <Game/Field/cBlock.h>
#include <Game/Field/FieldData.h>

namespace Game
{
namespace Field
{
class cChunk
{
public:

    cChunk();
    cChunk( int x, int z );
    ~cChunk();

    void setup();
    void update();
    void draw();

    cBlock& getBlock( int x, int y, int z );
    cBlock& getBlock( ci::ivec3 c );
    BlockMap& getBlocks();

    void add( const std::vector<ci::vec3>& vertices,
              const std::vector<uint32_t>& indices,
              const std::vector<ci::vec2>& uvs,
              const std::vector<ci::vec3>& normals );

    void breakBlock( ci::ivec3 c );

    ci::ivec3 getCell() { return mChunkCell; }

    bool createMainCall();
    ci::TriMeshRef createTriMesh();
    void createBlocks();

    bool isOutOfRange( ci::ivec3 c );


    ci::ivec3 mChunkCell;
    bool mIsLoaded;
    bool mIsDone;

private:

    std::vector<ci::vec3> mVertices;
    std::vector<uint> mIndices;
    std::vector<ci::vec2> mUv;
    std::vector<ci::vec3> mNormals;
    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVbo;

    BlockMap mBlocks;

};
}
}
