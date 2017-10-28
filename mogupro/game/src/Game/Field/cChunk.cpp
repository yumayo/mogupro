#include <Game/Field/cChunk.h>
#include <Game/Field/cUnderGround.h>
#include <Game/Field/cChunkMeshBuilder.h>
#include <Utility/cTimeMeasurement.h>
#include <Utility/cString.h>

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

ci::ivec3 cChunk::getCell()
{
    return mChunkCell;
}

std::shared_ptr<cBlock> cChunk::getBlock( int x, int y, int z )
{
    return getBlock( ci::ivec3( x, y, z ) );
}

std::shared_ptr<cBlock> cChunk::getBlock( ci::ivec3 c )
{
    if ( isOutOfRange( c ) )
    {
        auto world_pos = toWorldPosition( c );
        return mUnderGround->getBlock( world_pos );
    }
    return mBlocks[getIndex( c )];
}

cChunk & cChunk::getChunk( ci::ivec3 block_cell )
{
    if ( isOutOfRange( block_cell ) )
    {
        auto world_pos = toWorldPosition( block_cell );
        return mUnderGround->getChunk( world_pos );
    }
    return *this;
}

void cChunk::setBlock( ci::ivec3 c, std::shared_ptr<cBlock> block )
{
    if ( isOutOfRange( c ) )
    {
        auto world_pos = toWorldPosition( c );
        mUnderGround->setBlock( world_pos, block );
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
        vec3 vertex;
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

cChunk& cChunk::breakBlock( ci::ivec3 c )
{
    auto& block = getBlock( c );
    auto& chunk = getChunk( c );
    if ( block->mIsActive == false )
        return *this;
    block->mIsActive = false;
    block->toBreak();

    chunk.mIsBlockBroken = true;

    return chunk;

    // Vboを丸ごと構築するのでindex書き換えはしなくなってしまった。
    // 重くなったら要検討

    //auto id = block.mId;
    //auto indices = std::vector<uint>( block.mIndicesNum.size() * 4 );
    //for ( uint i = 0; i < block.mIndicesNum.size(); i++ )
    //{
    //    auto k = block.mIndicesNum[i];
    //    mIndices[k] = 0;
    //}
    //auto vbo = mVbo->getIndexVbo();
    //// VboRefの中でIndicesが4倍されるので、変更箇所を4倍する
    //vbo->bufferSubData( block.mIndicesNum[0] * 4, block.mIndicesNum.size() * 4, &indices[0] );

}

void cChunk::reBuildStart()
{
    if ( mIsBlockBroken == false )
        return;
    if ( mIsLoading )
        return;
    clearMesh();
    mHasBuild = true;
}

void cChunk::reBuildMesh()
{
    if ( mHasBuild == false )
        return;
    if ( mIsLoading )
        return;
    mIsLoading = true;

    buildMesh();
}

void cChunk::buildMesh()
{
    cChunkMeshBuilder builder( *this );
    mHasBuildCompleted = builder.buildMesh();
}

bool cChunk::reLoading()
{
    if ( mHasBuildCompleted == false )
        return false;
    if ( mHasBuild == false )
        return false;

    if ( mVbo != nullptr )
    {
        mIsLoading = false;
        mHasBuild = false;
        mIsBlockBroken = false;
        mHasBuildCompleted = false;

        mVbo = gl::VboMesh::create( *mMesh );
        return true;
    }
    return false;
}

bool cChunk::createMainCall()
{
    // 計算途中だったらはじく
    if ( mIsDone == false )
        return false;

    // vboを再構築したらはじく
    if ( reLoading() )
        return false;

    // vboが生成済みだったらはじく
    if ( mVbo != nullptr )
        return false;

    if ( mMesh == nullptr )
        return false;

    mVbo = gl::VboMesh::create( *mMesh );
    for ( auto& block : mBlocks )
        block->setup();

    cTimeMeasurement::getInstance()->make();
    auto t = cTimeMeasurement::getInstance()->deltaTime();

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
                std::shared_ptr<cBlock> block =
                    std::make_shared<cBlock>( position, (float) BLOCK_SIZE, id++ );

                mBlocks[getIndex( x, y, z )] = block;
            }
}

void cChunk::clearMesh()
{
    mIndicesIndex = 0;
    if ( mVbo != nullptr )
        if ( mMesh != nullptr )
            mMesh->clear();
}

ci::ivec3 cChunk::toWorldPosition( ci::ivec3 c )const
{
    return  mChunkCell * CHUNK_SIZE + c;
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
