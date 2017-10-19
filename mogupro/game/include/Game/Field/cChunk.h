#pragma once
#include <Game/Field/cBlock.h>
namespace Game
{
namespace Field
{
class cChunk
{
public:

    cChunk();
    ~cChunk();

    ci::TriMeshRef createTriMesh();

    std::vector<ci::vec3> mVertices;
    std::vector<uint> mIndices;
    std::vector<ci::vec2> mUv;
    std::vector<ci::vec3> mNormals;
    ci::TriMeshRef mMesh;
};
}
}
