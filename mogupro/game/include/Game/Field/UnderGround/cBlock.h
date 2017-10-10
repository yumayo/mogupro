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

    cBlock(const ci::vec3 position, const float scale);
    ~cBlock();

    void update();
    void draw();
    void drawMesh();
    void setupDrawSide(std::vector<int>* draw_side);

    bool mIsActive;

private:
    ci::vec3 mPosition;
    float mScale;

    std::vector<ci::vec3> mVertices;
    std::vector<uint32_t> mIndices;
    std::vector<ci::vec2> mUv;
    ci::TriMesh mMesh;
    ci::gl::VboMeshRef mVboMesh;
};
}
}
}



