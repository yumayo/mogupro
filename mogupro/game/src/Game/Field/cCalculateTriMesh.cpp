#include <Game/Field/cCalculateTriMesh.h>
#include <Game/Field/cChunk.h>
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
    {
        if ( vertices[i] == vtx
             && normals[i] == normal )
        {
            // 全部一致
            return i;
        }
    }
    return -1;
}
cChunk calcChunk( const ci::ivec3& chunk_size,
                  const ci::ivec2& chunk_num,
                  const uint& count,
                  const float& block_scale )
{
    cChunk chunk;

    vec3 offset_vertices = vec3( chunk_num.x * block_scale * chunk_size.x,
                                 0,
                                 chunk_num.y * block_scale * chunk_size.z );
    uint offset_indices =
        cBlock::cube_indices_size * count * multiplyIvec3( chunk_size );

    // UV
    auto uvs = getUv( 6 );
    vec3 prev_position = vec3( 0 );

    for ( uint z = 0; z < chunk_size.z; z++ )
    {
        for ( uint y = 0; y < chunk_size.y; y++ )
        {
            for ( uint x = 0; x < chunk_size.x; x++ )
            {
                auto position = vec3( x * block_scale,
                                      y * block_scale,
                                      z * block_scale );
                offset_vertices += position - prev_position;
                prev_position = position;

                // DrawElements
                std::vector<ci::vec3> vertices;
                std::vector<uint> indices;
                std::vector<ci::vec3> normals;

                for ( int i = 0; i < 36; ++i )
                {
                    vec3 vertex = vec3( cube_vtx[i * 3 + 0],
                                        cube_vtx[i * 3 + 1],
                                        cube_vtx[i * 3 + 2] ) + offset_vertices;
                    vec3 normal = vec3( cube_normal[i * 3 + 0],
                                        cube_normal[i * 3 + 1],
                                        cube_normal[i * 3 + 2] );

                    int id = getIndex( vertex, normal,
                                       vertices, normals );

                    if ( id < 0 )
                    {
                        indices.emplace_back( vertices.size() + offset_indices );
                        vertices.emplace_back( vertex );
                        normals.emplace_back( normal );
                    }
                    else
                        indices.emplace_back( id + offset_indices );
                }

                offset_indices += cBlock::cube_indices_size;

                std::copy( vertices.begin(), vertices.end(), std::back_inserter( chunk.mVertices ) );
                std::copy( indices.begin(), indices.end(), std::back_inserter( chunk.mIndices ) );
                std::copy( normals.begin(), normals.end(), std::back_inserter( chunk.mNormals ) );
                std::copy( uvs.begin(), uvs.end(), std::back_inserter( chunk.mUv ) );
            }
        }
    }

    chunk.createTriMesh();
    return chunk;
}
}
}
