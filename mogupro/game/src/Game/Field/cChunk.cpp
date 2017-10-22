#include <Game/Field/cChunk.h>
#include <Game/Field/cUnderGround.h>
#include <Utility/cTimeMeasurement.h>
using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
cChunk::cChunk() :
    mChunkCell( ci::ivec3( -1, -1, -1 ) )
{
    mMesh = TriMesh::create();
}

cChunk::cChunk( int x, int z, cUnderGround* under_ground ) :
    mChunkCell( ci::ivec3( x, 0, z ) )
    , mUnderGround( under_ground )
{
    mMesh = TriMesh::create();
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
    {
        return cBlock();
    }
    return mBlocks[getIndex( c )];
}

std::array<cBlock, CHUNK_VOLUME> & cChunk::getBlocks()
{
    return mBlocks;
}

void cChunk::setBlock( ci::ivec3 c, cBlock block )
{
    if ( isOutOfRange( c ) )
    {
        auto world_position = toWorldPosition( c );
        mUnderGround->setBlock( world_position, block );
        return;
    }
    mBlocks[getIndex( c )] = block;
}

void cChunk::addFace( const std::array<GLfloat, 12>& block_face,
                      const std::array<ci::vec2, 4> &texture_coords,
                      const ci::ivec3 & chunk_position,
                      const ci::vec3 & block_position )
{
    mMesh->appendTexCoords0( &texture_coords[0], 4 );
    for ( int i = 0, index = 0; i < 4; i++ )
    {
        vec3 vertex ;
        vertex.x += block_face[index++] + block_position.x;
        vertex.y += block_face[index++] + block_position.y;
        vertex.z += block_face[index++] + block_position.z;
        mMesh->appendPosition( vertex );
    }

    auto & indices = mMesh->getIndices();
    indices.insert( indices.end(),
    {
        mIndicesIndex,
        mIndicesIndex + 1,
        mIndicesIndex + 2,

        mIndicesIndex + 2,
        mIndicesIndex + 3,
        mIndicesIndex
    } );
    mIndicesIndex += 4;
}

void cChunk::breakBlock( ci::ivec3 c )
{
    auto& block = getBlock( c );
    if ( block.mIsActive == false )
        return;
    block.mIsActive = false;

    //auto id = block.mId;
    //auto indices = std::vector<uint>( block.mIndicesNum.size() * 4 );
    //for ( uint i = 0; i < block.mIndicesNum.size(); i++ )
    //{
    //    auto k = block.mIndicesNum[i];
    //    mIndices[k] = 0;
    //}
    //auto vbo = mVbo->getIndexVbo();
    //// VboRef‚Ì’†‚ÅIndices‚ª4”{‚³‚ê‚é‚Ì‚ÅA•ÏX‰ÓŠ‚ð4”{‚·‚é
    //vbo->bufferSubData( block.mIndicesNum[0] * 4, block.mIndicesNum.size() * 4, &indices[0] );
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
        block.setup();

    cTimeMeasurement::getInstance()->make();
    auto t = cTimeMeasurement::getInstance()->deltaTime();

    console() << std::endl << std::endl;
    console() << "Field create time : " << t << "pos " << mChunkCell << std::endl;
    console() << std::endl << std::endl;
    return true;
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
                cBlock block = cBlock( position, (float) BLOCK_SIZE, id++ );

                mBlocks[getIndex( x, y, z )] = block;
            }
}

ci::ivec3 cChunk::toWorldPosition( ci::ivec3 c )const
{
    return mChunkCell * CHUNK_SIZE + c;
}

bool cChunk::isOutOfRange( ci::ivec3 c )
{
    if ( c.x >= CHUNK_SIZE || c.y >= CHUNK_SIZE || c.z >= CHUNK_SIZE )
        return true;
    if ( c.x < 0 || c.y < 0 || c.z < 0 )
        return true;
    return false;
}

int cChunk::getIndex( ci::ivec3 c )
{
    return getIndex( c.x, c.y, c.z );
}

int cChunk::getIndex( int x, int y, int z )
{
    return x + y * CHUNK_AREA + z * CHUNK_SIZE;
}
}
}
