#pragma once
#include <cinder/app/AppBase.h>
#include <cinder/gl/gl.h>
#include <Collision/cAABBCollider.h>
using uint = uint32_t;
namespace Game
{
namespace Field
{
class cBlock
{
public:

    cBlock( const ci::vec3& position, const float& scale, const uint& num );
    ~cBlock();

    void update();
    void draw();

public: // Method

    void createSide( std::vector<ci::vec3>& vertices,
                     std::vector<uint>& indices,
                     std::vector<ci::vec2>& uvs,
                     std::vector<ci::vec3>& normals );
    void clear();
    void toBreak();

public: // Member

    ci::vec3 mPosition;
    float mScale;
    bool mIsActive;
    uint mNum;
    Collision::cAABBCollider mCollider;
    std::vector<int> mDrawSide;

    std::vector<uint> mVerticesNum;
    std::vector<uint> mIndicesNum;
    std::vector<uint> mUvNum;
    std::vector<uint> mNormalsNum;

};
}
}