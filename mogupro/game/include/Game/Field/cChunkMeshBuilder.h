#pragma once
#include <Game/Field/FieldData.h>
namespace Game
{
namespace Field
{

class cChunkMeshBuilder
{
public:
    cChunkMeshBuilder( cChunkLayer& chunk_layer );
    ~cChunkMeshBuilder();

    bool buildMesh();

private:

    void tryAddFaceToMesh( const std::array<GLfloat, 12>& block_face,
                           const std::array<GLfloat, 12>& block_normal,
                           const std::array<ci::vec2, 4>& tex_coords,
                           const ci::vec3 & position,
                           const ci::ivec3 & block_facing );

    void addFace( const std::array<GLfloat, 12>& block_face,
                  const std::array<GLfloat, 12>& block_normal,
                  const std::array<ci::vec2, 4>& tex_coords,
                  const ci::vec3 & block_position );

private:

    cChunkLayer* mChunkLayer;

};
}
}
