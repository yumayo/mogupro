#pragma once
#include <cinder/app/AppBase.h>
#include <cinder/gl/gl.h>

namespace Game
{
namespace Field
{
namespace UnderGround
{
class cBlock
{
public:

    cBlock( const ci::vec3 position, const float scale );
    ~cBlock();

    void update();
    void draw();
    void drawMesh();
    void setupDrawSide( const std::vector<int>& draw_side );
    void clear();

    void toBreak();

    bool mIsActive;

private:
    ci::vec3 mPosition;
    float mScale;

    std::vector<ci::vec3> mVertices;
    std::vector<uint32_t> mIndices;
    std::vector<ci::vec2> mUv;
    ci::TriMeshRef mMesh;
    ci::gl::VboMeshRef mVboMesh;
};
}
}
}



