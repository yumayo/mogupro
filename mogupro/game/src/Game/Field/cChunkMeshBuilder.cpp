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

const std::array<GLfloat, 12> front_normal
{
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
};
static std::array<GLfloat, 12> back_normal
{
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
};
static std::array<GLfloat, 12> left_normal
{
   -1.0f, 0.0f, 0.0f,
   -1.0f, 0.0f, 0.0f,
   -1.0f, 0.0f, 0.0f,
   -1.0f, 0.0f, 0.0f,
};
static std::array<GLfloat, 12> right_normal
{
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
};
static std::array<GLfloat, 12> top_normal
{
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
};
static std::array<GLfloat, 12> bottom_normal
{
    0.0f, -1.0f, 0.0f ,
    0.0f, -1.0f, 0.0f ,
    0.0f, -1.0f, 0.0f ,
    0.0f, -1.0f, 0.0f ,
};

const std::array<ci::vec2, 4> texture_coords_dirt
{
    ci::vec2( 0.0  ,0.0 ),
    ci::vec2( 0.0  ,1.0 ),
    ci::vec2( 0.25 ,1.0 ),
    ci::vec2( 0.25 ,0.0 ),
};
const std::array<ci::vec2, 4> texture_coords_rock
{
    ci::vec2( 0.25 ,0.0 ),
    ci::vec2( 0.25 ,1.0 ),
    ci::vec2( 0.5  ,1.0 ),
    ci::vec2( 0.5  ,0.0 ),
};
const std::array<ci::vec2, 4> texture_coords_sand
{
    ci::vec2( 0.5  ,0.0 ),
    ci::vec2( 0.5  ,1.0 ),
    ci::vec2( 0.75 ,1.0 ),
    ci::vec2( 0.75 ,0.0 ),
};
const std::array<ci::vec2, 4> texture_coords_mud
{
    ci::vec2( 0.75 ,0.0 ),
    ci::vec2( 0.75 ,1.0 ),
    ci::vec2( 1.0  ,1.0 ),
    ci::vec2( 1.0  ,0.0 ),
};

std::array<ci::vec2, 4> getTexCoordsFromBlockType( const BlockType& type )
{
    switch ( type )
    {
        case BlockType::AIR:
            break;
        case BlockType::NORMAL:
            return texture_coords_dirt;
        case BlockType::HARD:
            break;
        case BlockType::UNBREAKING:
            return texture_coords_rock;
    }
    return texture_coords_dirt;
}

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

                if ( block->isActive() == false )
                    continue;

                std::array<ci::vec2, 4> tex_coords;
                tex_coords = getTexCoordsFromBlockType( block->getType() );

                auto position = block->getPosition();

                directions.update( x, y, z );

                tryAddFaceToMesh( front_face, front_normal, tex_coords, position, directions.front );
                tryAddFaceToMesh( back_face, back_normal, tex_coords, position, directions.back );
                tryAddFaceToMesh( left_face, left_normal, tex_coords, position, directions.left );
                tryAddFaceToMesh( right_face, right_normal, tex_coords, position, directions.right );
                if ( mChunkLayer->getChunkCell().y != 0 || y != 0 )
                    tryAddFaceToMesh( bottom_face, bottom_normal, tex_coords, position, directions.down );

                if ( block->getType() != BlockType::UNBREAKING &&
                     mChunkLayer->getChunkCell().y == CHUNK_RANGE_Y - 1 && y >= CHUNK_SIZE - 1 )
                    tex_coords = texture_coords_mud;
                tryAddFaceToMesh( top_face, top_normal, tex_coords, position, directions.up );
            }
        }
    }
    return true;
}

void cChunkMeshBuilder::tryAddFaceToMesh( const std::array<GLfloat, 12>& block_face,
                                          const std::array<GLfloat, 12>& block_normal,
                                          const std::array<ci::vec2, 4>& tex_coords,
                                          const ci::vec3 & position,
                                          const ci::ivec3 & block_facing )
{
    auto block = mChunkLayer->getBlock( block_facing );
    if ( block->isActive() == false )
        addFace( block_face, block_normal, tex_coords, position );
}

void cChunkMeshBuilder::addFace( const std::array<GLfloat, 12>& block_face,
                                 const std::array<GLfloat, 12>& block_normal,
                                 const std::array<ci::vec2, 4>& tex_coords,
                                 const ci::vec3 & block_position )
{
    mChunkLayer->addFace( block_face,
                          block_normal,
                          tex_coords,
                          block_position );
}

}
}