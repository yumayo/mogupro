#include <Game/Field/cChunk.h>
#include <Utility/cTimeMeasurement.h>
using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
cChunk::cChunk( int x, int z ) :
    mChunkCell( ci::ivec3( x, 0, z ) )
{
}
cChunk::~cChunk()
{

}
void cChunk::setup()
{
}
void cChunk::update()
{
}
void cChunk::draw()
{
    if ( mVbo != nullptr )
        gl::draw( mVbo );
}
cBlock & cChunk::getBlock( int x, int y, int z )
{
    return getBlock( ci::ivec3( x, y, z ) );
}
cBlock & cChunk::getBlock( ci::ivec3 c )
{
    return mBlocks[c];
}
BlockMap & cChunk::getBlocks()
{
    return mBlocks;
}
void cChunk::add( const std::vector<ci::vec3>& vertices,
                  const std::vector<uint32_t>& indices,
                  const std::vector<ci::vec2>& uvs,
                  const std::vector<ci::vec3>& normals )
{
    std::copy( vertices.begin(), vertices.end(), std::back_inserter( mVertices ) );
    std::copy( indices.begin(), indices.end(), std::back_inserter( mIndices ) );
    std::copy( uvs.begin(), uvs.end(), std::back_inserter( mUv ) );
    std::copy( normals.begin(), normals.end(), std::back_inserter( mNormals ) );
}
void cChunk::breakBlock( ci::ivec3 c )
{
    if ( isOutOfRange( c ) )
        return;

    if ( mBlocks.find( c ) == mBlocks.end() )
        return;

    auto& block = mBlocks[c];
    auto indices = std::vector<uint>( block.mIndicesNum.size() * 4 );
    for ( uint i = 0; i < block.mIndicesNum.size(); i++ )
    {
        auto k = block.mIndicesNum[i];
        mIndices[k] = 0;
    }
    auto vbo = mVbo->getIndexVbo();
    // VboRefの中でIndicesが4倍されるので、変更箇所を4倍する
    vbo->bufferSubData( block.mIndicesNum[0] * 4, block.mIndicesNum.size() * 4, &indices[0] );
}
ci::gl::VboMeshRef cChunk::createVboMesh()
{
    auto mesh = gl::VboMesh::create( *createTriMesh() );
    mVbo = mesh;
    return mVbo;
}
ci::TriMeshRef cChunk::createTriMesh()
{
    mMesh = ci::TriMesh::create();
    if ( mVertices.size() > 0 )
        mMesh->appendPositions( &mVertices[0], mVertices.size() );
    if ( mIndices.size() > 0 )
        mMesh->appendIndices( &mIndices[0], mIndices.size() );
    if ( mUv.size() > 0 )
        mMesh->appendTexCoords0( &mUv[0], mUv.size() );
    if ( mNormals.size() > 0 )
        mMesh->appendNormals( &mNormals[0], mNormals.size() );
    return mMesh;
}
void cChunk::createBlocks()
{
    int id = 0;
    vec3 offset = mChunkCell * CHUNK_SIZE;
    for ( int z = 0; z < CHUNK_SIZE; z++ )
        for ( int y = 0; y < CHUNK_SIZE; y++ )
            for ( int x = 0; x < CHUNK_SIZE; x++ )
            {
                vec3 position = vec3( x * BLOCK_SIZE,
                                      y * BLOCK_SIZE,
                                      z * BLOCK_SIZE ) + offset;// +OFFSET_POSITION;
                cBlock block = cBlock( position, (float) BLOCK_SIZE );
                block.calcMeshIndexData( id++ );

                mBlocks[ivec3( x, y, z )] = block;
            }
}
bool cChunk::isOutOfRange( ci::ivec3 c )
{
    if ( c.x >= CHUNK_SIZE || c.y >= CHUNK_SIZE || c.z >= CHUNK_SIZE )
        return true;
    if ( c.x < 0 || c.y < 0 || c.z < 0 )
        return true;
    //if ( c.y > CHUNK_SIZE * 8 )
        //return true;
    return false;
}
}
}
