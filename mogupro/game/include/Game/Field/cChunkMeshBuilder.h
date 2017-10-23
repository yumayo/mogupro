#pragma once
#include <Game/Field/FieldData.h>
namespace Game
{
namespace Field
{

class cBlock;
class cChunk;

class cChunkMeshBuilder
{
public:
    cChunkMeshBuilder( cChunk& chunk );
    ~cChunkMeshBuilder();

    bool buildMesh();

private:

    void tryAddFaceToMesh( const std::array<GLfloat, 12>& block_face,
                           const ci::vec3 & position,
                           const ci::ivec3 & block_facing );

    void addFace( const std::array<GLfloat, 12>& block_face,
                  const ci::ivec3 & chunk_position,
                  const ci::vec3 & block_position );

private:

    cChunk* mChunk;

};
}
}
