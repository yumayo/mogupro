#pragma once
#include <Game/Field/UnderGround/cBlock.h>

namespace Game
{
namespace Field
{
namespace UnderGround
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
    bool blockVertexBlend();

private:

    std::vector<std::vector<std::vector<std::shared_ptr<cBlock>>>> blocks;
    int num;
    float offset;
    float scale;

    std::vector<ci::vec3> mVertices;
    std::vector<uint32_t> mIndices;
    std::vector<ci::vec2> mUv;
    std::vector<ci::vec3> mNormals;
    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVboMesh;

};
}
}
}



