#include <Game/Field/cChunk.h>
#include <Utility/cTimeMeasurement.h>
using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
cChunk::cChunk() :
    mChunkCell( ci::ivec3( -1, -1, -1 ) )
    , mIsLoaded( false )
    , mIsDone( false )
{
}
cChunk::cChunk( int x, int z ) :
    mChunkCell( ci::ivec3( x, 0, z ) )
    , mIsLoaded( false )
    , mIsDone( false )
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
    {
        gl::draw( mVbo );
    }
}
cBlock & cChunk::getBlock( int x, int y, int z )
{
    return getBlock( ci::ivec3( x, y, z ) );
}
cBlock & cChunk::getBlock( ci::ivec3 c )
{
    if ( isOutOfRange( c ) )
        return cBlock();
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
    auto& block = getBlock( c );
    if ( block.mIsActive == false )
        return;

    auto indices = std::vector<uint>( block.mIndicesNum.size() * 4 );
    for ( uint i = 0; i < block.mIndicesNum.size(); i++ )
    {
        auto k = block.mIndicesNum[i];
        mIndices[k] = 0;
    }
    auto vbo = mVbo->getIndexVbo();
    // VboRef‚Ì’†‚ÅIndices‚ª4”{‚³‚ê‚é‚Ì‚ÅA•ÏX‰ÓŠ‚ð4”{‚·‚é
    vbo->bufferSubData( block.mIndicesNum[0] * 4, block.mIndicesNum.size() * 4, &indices[0] );
}
bool cChunk::createMainCall()
{
    // vbo‚ª¶¬Ï‚Ý‚¾‚Á‚½‚ç‚Í‚¶‚­
    if ( mVbo != nullptr )
        return false;

    // ŒvŽZ“r’†‚¾‚Á‚½‚ç‚Í‚¶‚­
    if ( mIsDone == false )
        return false;

    if ( mMesh == nullptr )
        return false;
    mVbo = gl::VboMesh::create( *mMesh );
    for ( auto& block : mBlocks )
        block.second.setup();

    cTimeMeasurement::getInstance()->make();
    auto t = cTimeMeasurement::getInstance()->deltaTime();

    //console() << std::endl << std::endl;
    //console() << "Field create time : " << t << "pos " << mChunkCell << std::endl;
    //console() << std::endl << std::endl;
    return true;
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
