#pragma once
#include <Game/Field/cBlock.h>
using uint = uint32_t;
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

public:

    bool blockBreak( const ci::vec3& position, const float& radius );
    bool isOutOfRange( const ci::ivec3& cell_num );

private:
    bool blockDigged( const  ci::ivec3& cell_num );

    void blockMeshBlend( std::shared_ptr<cBlock> b );
    void blockMeshErase( std::shared_ptr<cBlock> b );
    bool createMesh();
    void blockClear();
private:

    std::vector<std::vector<std::vector<std::shared_ptr<cBlock>>>> blocks;
    int mNum;
    int mHeight;
    float mIntervalOffset;
    float mScale;
    ci::vec3 mOffset;

    std::vector<ci::vec3> mVertices;
    std::vector<uint> mIndices;
    std::vector<ci::vec2> mUv;
    std::vector<ci::vec3> mNormals;
    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVboMesh;

};
}
}



