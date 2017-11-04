#include <Game/Field/cChunkMeshBuilder.h>
#include <Game/Field/cBlock.h>
#include <Game/Field/cChunk.h>
#include <array>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{

const std::array<GLfloat, 12> front_face
{
    -BLOCK_SIZE / 2,  -BLOCK_SIZE / 2, BLOCK_SIZE / 2,
    BLOCK_SIZE / 2, -BLOCK_SIZE / 2, BLOCK_SIZE / 2,
    BLOCK_SIZE / 2,  BLOCK_SIZE / 2, BLOCK_SIZE / 2,
    -BLOCK_SIZE / 2,  BLOCK_SIZE / 2, BLOCK_SIZE / 2,
};
const std::array<GLfloat, 12> back_face
{
    BLOCK_SIZE / 2, -BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
    -BLOCK_SIZE / 2, -BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
    -BLOCK_SIZE / 2,  BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
    BLOCK_SIZE / 2,  BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
};
const std::array<GLfloat, 12> left_face
{
    -BLOCK_SIZE / 2, -BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
    -BLOCK_SIZE / 2, -BLOCK_SIZE / 2,  BLOCK_SIZE / 2,
    -BLOCK_SIZE / 2,  BLOCK_SIZE / 2,  BLOCK_SIZE / 2,
    -BLOCK_SIZE / 2,  BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
};
const std::array<GLfloat, 12> right_face
{
    BLOCK_SIZE / 2, -BLOCK_SIZE / 2,  BLOCK_SIZE / 2,
    BLOCK_SIZE / 2, -BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
    BLOCK_SIZE / 2,  BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
    BLOCK_SIZE / 2,  BLOCK_SIZE / 2,  BLOCK_SIZE / 2,
};
const std::array<GLfloat, 12> top_face
{
    -BLOCK_SIZE / 2, BLOCK_SIZE / 2,  BLOCK_SIZE / 2,
    BLOCK_SIZE / 2, BLOCK_SIZE / 2,  BLOCK_SIZE / 2,
    BLOCK_SIZE / 2, BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
    -BLOCK_SIZE / 2, BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
};
const std::array<GLfloat, 12> bottom_face
{
    -BLOCK_SIZE / 2, -BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
    BLOCK_SIZE / 2, -BLOCK_SIZE / 2, -BLOCK_SIZE / 2,
    BLOCK_SIZE / 2, -BLOCK_SIZE / 2,  BLOCK_SIZE / 2,
    -BLOCK_SIZE / 2, -BLOCK_SIZE / 2,  BLOCK_SIZE / 2,
};

const std::array<ci::vec2, 4> texture_coords
{
    ci::vec2( 0.0 ,0.0 ),
    ci::vec2( 0.0 ,1.0 ),
    ci::vec2( 1.0 ,1.0 ),
    ci::vec2( 1.0 ,0.0 ),
};

struct AdjacentBlockPositions
{
    void update( int x, int y, int z )
    {
        up = { x,     y + 1,  z };
        down = { x,     y - 1,  z };
        left = { x - 1, y,      z };
        right = { x + 1, y,      z };
        front = { x,     y,      z + 1 };
        back = { x,     y,      z - 1 };
    }

    ci::ivec3 up;
    ci::ivec3 down;
    ci::ivec3 left;
    ci::ivec3 right;
    ci::ivec3 front;
    ci::ivec3 back;
};

cChunkMeshBuilder::cChunkMeshBuilder( cChunkLayer& chunk_layer ) :
    mChunkLayer( &chunk_layer )
{

}
cChunkMeshBuilder::~cChunkMeshBuilder()
{

}

bool cChunkMeshBuilder::buildMesh()
{
    AdjacentBlockPositions directions;

    for ( int z = 0; z < CHUNK_SIZE; z++ )
    {
        for ( int y = 0; y < CHUNK_SIZE; y++ )
        {
            for ( int x = 0; x < CHUNK_SIZE; x++ )
            {
                auto block = mChunkLayer->getBlock( ivec3( x, y, z ) );

                if ( block->mIsActive == false )
                    continue;

                auto position = block->mPosition;

                directions.update( x, y, z );

                if ( mChunkLayer->getChunkCell().y != 0 || y != 0 )
                    tryAddFaceToMesh( bottom_face, position, directions.down );
                tryAddFaceToMesh( top_face, position, directions.up );
                tryAddFaceToMesh( left_face, position, directions.left );
                tryAddFaceToMesh( right_face, position, directions.right );
                tryAddFaceToMesh( front_face, position, directions.front );
                tryAddFaceToMesh( back_face, position, directions.back );
            }
        }
    }
    return true;
}

void cChunkMeshBuilder::tryAddFaceToMesh( const std::array<GLfloat, 12>& block_face,
                                          const ci::vec3 & position,
                                          const ci::ivec3 & block_facing )
{
    auto block = mChunkLayer->getBlock( block_facing );
    if ( block->mIsActive == false )
        addFace( block_face, position );
}

void cChunkMeshBuilder::addFace( const std::array<GLfloat, 12>& block_face,
                                 const ci::vec3 & block_position )
{
    mChunkLayer->addFace( block_face,
                          texture_coords,
                          block_position );
}

}
}