#pragma once
#include <cinder/app/AppBase.h>
#include <cinder/gl/gl.h>

namespace Game
{
namespace Field
{
class cBlock
{
public:

    cBlock( const ci::vec3 position, const float scale );
    ~cBlock();

    void update();
    void draw();
    void drawMesh();

    void createSide( const  int& offset_index = 0 );
    void clear();

    void toBreak();

    bool mIsActive;
    std::vector<int> mDrawSide;

public:
    ci::vec3 mPosition;
    float mScale;

    std::vector<ci::vec3> mVertices;
    std::vector<uint32_t> mIndices;
    std::vector<ci::vec2> mUv;
    std::vector<ci::vec3> mNormals;
    ci::TriMeshRef mMesh;
};
}
}