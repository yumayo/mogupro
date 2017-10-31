#include <Game/Field/cChunkLayer.h>
#include <Game/Field/cChunkMeshBuilder.h>
#include <Game/Field/cBlock.h>
#include <Game/Field/cChunk.h>
#include <Game/Field/cUnderGround.h>
#include <Utility/cString.h>
using namespace ci;
using namespace ci::app;
namespace Game
{
namespace Field
{
cChunkLayer::cChunkLayer()
{
}

cChunkLayer::cChunkLayer( const int& height, cChunk* chunk, cUnderGround* under_ground ) :
    mHeight( height )
    , mChunk( chunk )
    , mUnderGround( under_ground )
{
    mMesh = TriMesh::create();
}

cChunkLayer::~cChunkLayer()
{

}

void cChunkLayer::setup()
{
}

void cChunkLayer::draw()
{
    if ( mVbo != nullptr )
        gl::draw( mVbo );
}

cBlock* cChunkLayer::getBlock( const int& x, const  int& y, const int& z )
{
    return getBlock( ci::ivec3( x, y, z ) );
}

cBlock* cChunkLayer::getBlock( const ci::ivec3& c )
{
    if ( outOfBounds( c.x ) ||
         outOfBounds( c.y ) ||
         outOfBounds( c.z ) )
    {
        auto world_pos = toWorldPosition( c );
        return mUnderGround->getBlock( world_pos );
    }
    return mBlocks[getIndex( c )].get();
}

cChunk* cChunkLayer::getChunk( const ci::ivec3 & block_cell )
{
    if ( outOfBounds( block_cell.x ) ||
         outOfBounds( block_cell.z ) )
    {
        auto world_pos = toWorldPosition( block_cell );
        return mUnderGround->getChunk( world_pos );
    }
    return mChunk;
}

cChunkLayer* cChunkLayer::getChunkLayer( const int & height )
{
    if ( height != mHeight )
        return mChunk->getChunkLayer( height );
    return this;
}

ci::ivec3 cChunkLayer::getChunkCell()
{
    auto c = mChunk->getCell();
    return ivec3( c.x, mHeight, c.z );
}

int cChunkLayer::getIndex( const ci::ivec3& c )
{
    return getIndex( c.x, c.y, c.z );
}

int cChunkLayer::getIndex( const int& x, const  int& y, const int& z )
{
    return x + y * CHUNK_AREA + z * CHUNK_SIZE;
}

void cChunkLayer::addFace( const std::array<GLfloat, 12>& block_face,
                           const std::array<ci::vec2, 4> &texture_coords,
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

cChunkLayer* cChunkLayer::breakBlock( ci::ivec3 c )
{
    auto block = getBlock( c );
    if ( block->mIsActive == false )
        return nullptr;
    block->mIsActive = false;
    block->toBreak();

    mIsBlockBroken = true;

    return this;
}

void cChunkLayer::reBuildStart()
{
    if ( mIsBlockBroken == false )
        return;
    if ( mIsLoading )
        return;
    clearMesh();
    mHasBuild = true;
}

void cChunkLayer::reBuildMesh()
{
    if ( mHasBuild == false )
        return;
    if ( mIsLoading )
        return;
    mIsLoading = true;

    buildMesh();
}

void cChunkLayer::buildMesh()
{
    cChunkMeshBuilder builder( *this );
    mHasBuildCompleted = builder.buildMesh();
}

bool cChunkLayer::reLoading()
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

bool cChunkLayer::createMainCall()
{
    // �v�Z�r����������͂���
    if ( mIsDone == false )
        return false;

    // vbo���č\�z������͂���
    if ( reLoading() )
        return false;

    // vbo�������ς݂�������͂���
    if ( mVbo != nullptr )
        return false;

    if ( mMesh == nullptr )
        return false;

    mVbo = gl::VboMesh::create( *mMesh );

    return true;
}

void cChunkLayer::createBlocks()
{
    int id = 0;
    vec3 offset = mChunk->getCell() * CHUNK_SIZE;
    offset.y = mHeight * CHUNK_SIZE;

    for ( int z = 0; z < CHUNK_SIZE; z++ )
        for ( int y = 0; y < CHUNK_SIZE; y++ )
            for ( int x = 0; x < CHUNK_SIZE; x++ )
            {
                vec3 position = vec3( x * BLOCK_SIZE,
                                      y * BLOCK_SIZE,
                                      z * BLOCK_SIZE ) + offset;// +OFFSET_POSITION;
                cBlockRef block;
                block = std::make_shared<cBlock>( position, (float) BLOCK_SIZE, id++ );

                // ��ԏ�̃��C���[�̓u���b�N�𐶐����Ȃ�
                if ( mHeight >= CHUNK_RANGE_Y)
                    block->mIsActive = false;

                // Colider����
                block->setup();
                mBlocks[getIndex( x, y, z )] = block;
            }
}

void cChunkLayer::clearMesh()
{
    mIndicesIndex = 0;
    if ( mVbo != nullptr )
        if ( mMesh != nullptr )
            mMesh->clear();
}

ci::ivec3 cChunkLayer::toWorldPosition( ci::ivec3 c )
{
    return getChunkCell() * CHUNK_SIZE + c;
}

bool cChunkLayer::outOfBounds( const int & v )const
{
    return ( v >= CHUNK_SIZE || v < 0 );
}

}
}
