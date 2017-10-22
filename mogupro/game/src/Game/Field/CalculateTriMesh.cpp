#include <Game/Field/CalculateTriMesh.h>
#include <Game/Field/cChunk.h>
#include <Game/Field/cChunkHolder.h>
using namespace ci;
namespace Game
{
namespace Field
{
static const GLfloat cube_vtx[] = {
    // 前
    //  x,     y,    z,
    -0.5f,  0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,
    0.5f,  0.5f, 0.5f,
    -0.5f,  0.5f, 0.5f,
    -0.5f, -0.5f, 0.5f,
    0.5f, -0.5f, 0.5f,

    // 後
    -0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f,  0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,

    // 右
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f,  0.5f, -0.5f,
    0.5f,  0.5f,  0.5f,
    0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,

    // 左
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,
    -0.5f, -0.5f, -0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f,  0.5f,  0.5f,
    -0.5f,  0.5f, -0.5f,

    // 上
    -0.5f, 0.5f, -0.5f,
    0.5f, 0.5f,  0.5f,
    0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f, -0.5f,
    -0.5f, 0.5f,  0.5f,
    0.5f, 0.5f,  0.5f,

    // 底
    -0.5f, -0.5f,  0.5f,
    0.5f, -0.5f, -0.5f,
    0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f,  0.5f,
    -0.5f, -0.5f, -0.5f,
    0.5f, -0.5f, -0.5f,
};

static const GLfloat cube_normal[] = {
    // 前
    // x,    y,    z,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,
    0.0f, 0.0f, 1.0f,

    // 後
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,
    0.0f, 0.0f, -1.0f,

    // 右
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,
    1.0f, 0.0f, 0.0f,

    // 左
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,
    -1.0f, 0.0f, 0.0f,

    // 上
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,
    0.0f, 1.0f, 0.0f,

    // 下
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
    0.0f, -1.0f, 0.0f,
};

std::vector<ci::vec2> getUv( int side_num )
{
    std::vector<vec2> uv = {
        vec2( 0.0 ,1.0 ),
        vec2( 1.0 ,0.0 ),
        vec2( 0.0 ,0.0 ),
        vec2( 1.0 ,1.0 ), };

    std::vector<vec2> uvs;
    for ( int i = 0; i < side_num; i++ )
        std::copy( uv.begin(), uv.end(), std::back_inserter( uvs ) );

    return uvs;
}
int getIndex( const ci::vec3 & vtx, const ci::vec3 & normal,
              const std::vector<ci::vec3>& vertices,
              const std::vector<ci::vec3>& normals )
{
    for ( uint i = 0; i < ( vertices.size() ); ++i )

        if ( vertices[i] == vtx
             && normals[i] == normal )
            return i;
    return -1;
}
cChunk calcChunkData( cChunk chunk )
{
    auto& c = chunk.getBlocks();
    // UV
    auto uvs = getUv( 6 );
    uint offset_id = 0;

    for ( uint z = 0; z < CHUNK_SIZE; z++ )
    {
        for ( uint y = 0; y < CHUNK_SIZE; y++ )
        {
            for ( uint x = 0; x < CHUNK_SIZE; x++ )
            {
                auto b = c[ivec3( x, y, z )];
                if ( b.mIsActive == false )
                    continue;

                auto position = b.mPosition;

                // DrawElements
                std::vector<ci::vec3> vertices;
                std::vector<uint> indices;
                std::vector<ci::vec3> normals;

                for ( int i = 0; i < 36; ++i )
                {
                    vec3 vertex = vec3( cube_vtx[i * 3 + 0],
                                        cube_vtx[i * 3 + 1],
                                        cube_vtx[i * 3 + 2] ) + position;
                    vec3 normal = vec3( cube_normal[i * 3 + 0],
                                        cube_normal[i * 3 + 1],
                                        cube_normal[i * 3 + 2] );

                    int id = getIndex( vertex, normal,
                                       vertices, normals );

                    if ( id < 0 )
                    {
                        indices.emplace_back( vertices.size() + offset_id );
                        vertices.emplace_back( vertex );
                        normals.emplace_back( normal );
                    }
                    else
                        indices.emplace_back( id + offset_id );
                }

                offset_id += cBlock::cube_indices_size;
                chunk.add( vertices, indices, uvs, normals );
            }
        }
    }

    chunk.createTriMesh();
    return chunk;
}
}
}
