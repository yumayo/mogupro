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

    cBlock();
    cBlock( const ci::vec3& position, const float& scale, const uint& num );
    ~cBlock();

    void update();
    void draw();

public: // Method

    void calcMeshIndexData( uint num );
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

public: // Const

    static const uint cube_indices_size = 24;
    static const uint cube_indices_index_size = 36;
    static const uint cube_vertices_index_size = 24;
    static const uint cube_normals_index_size = 24;
    static const uint cube_uvs_index_size = 24;

};
}
}