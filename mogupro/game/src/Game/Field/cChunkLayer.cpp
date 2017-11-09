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
cChunkLayer::cChunkLayer() :
    mIsActive( true )
{
}

cChunkLayer::cChunkLayer( const int& height, cChunk* chunk, cUnderGround* under_ground ) :
    mHeight( height )
    , mChunk( chunk )
    , mUnderGround( under_ground )
    , mIsActive( true )
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
    {
        auto ctx = gl::context();
        const gl::GlslProg* curGlslProg = ctx->getGlslProg();


        //ctx->pushVao();
        ctx->getDefaultVao()->replacementBindBegin();
        mVbo->buildVao( curGlslProg );
        ctx->getDefaultVao()->replacementBindEnd();
        ctx->setDefaultShaderVars();
        mVbo->drawImpl();
        //ctx->popVao();
    }
    //gl::draw( mVbo );
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

cChunkLayer* cChunkLayer::getChunkLayer( const int & height )
{
    if ( height != mHeight )
        return mChunk->getChunkLayer( height );
    return this;
}

cChunkLayer * cChunkLayer::getChunkLayer( const ci::ivec3 & cell )
{
    if ( outOfBounds( cell.x ) ||
         outOfBounds( cell.y ) ||
         outOfBounds( cell.z ) )
    {
        auto world_pos = toWorldPosition( cell );
        return mUnderGround->getChunkLayer( world_pos );
    }
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
                           const std::array<GLfloat, 12>& block_normal,
                           const std::array<ci::vec2, 4> &texture_coords,
                           const ci::vec3 & block_position )
{
    mMesh->appendTexCoords0( &texture_coords[0], 4 );
    for ( int i = 0, index = 0; i < 4; i++ )
    {
        vec3 vertex( 0 );
        vertex.x += block_face[index++] + block_position.x;
        vertex.y += block_face[index++] + block_position.y;
        vertex.z += block_face[index++] + block_position.z;
        mMesh->appendPosition( vertex );
    }

    for ( int i = 0, index = 0; i < 4; i++ )
    {
        vec3 normal( 0 );
        normal.x += block_normal[index++];
        normal.y += block_normal[index++];
        normal.z += block_normal[index++];
        mMesh->appendNormal( normal );
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
    if ( block->getType() == BlockType::AIR )
        return nullptr;
    block->toBreak();

    auto layer = getChunkLayer( c );
    if ( layer->mIsActive == false )
        return this;
    layer->mIsBlockBroken = true;

    return layer;
}

cChunkLayer * cChunkLayer::breakBlock( cBlock * block, cChunkLayer* layer )
{
    if ( block->getType() == BlockType::AIR )
        return nullptr;
    block->toBreak();

    if ( layer->mIsActive == false )
        return nullptr;
    layer->mIsBlockBroken = true;

    return layer;
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
    if ( mIsActive == false )
    {
        mHasBuildCompleted = true;
        return;
    }
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
    vec3 offset;
    vec3 cell = mChunk->getCell();
    float x = cell.x * CHUNK_SIZE;
    float z = cell.z * CHUNK_SIZE;
    float y = mHeight * (float) CHUNK_SIZE;
    offset = vec3( x, y, z ) * BLOCK_SIZE;

    for ( int z = 0; z < CHUNK_SIZE; z++ )
        for ( int y = 0; y < CHUNK_SIZE; y++ )
            for ( int x = 0; x < CHUNK_SIZE; x++ )
            {
                vec3 position = vec3( x * BLOCK_SIZE,
                                      y * BLOCK_SIZE,
                                      z * BLOCK_SIZE ) + offset;
                cBlockRef block;
                block = std::make_shared<cBlock>( position, BLOCK_SIZE, id++ );

                // ��ԏ�̃��C���[�̓u���b�N�𐶐����Ȃ�
                if ( mHeight >= CHUNK_RANGE_Y )
                    block->setType( BlockType::AIR );


                // Collider����
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

ci::vec3 cChunkLayer::toWorldPosition( ci::ivec3 c )
{
    vec3 cell = getChunkCell() * CHUNK_SIZE;
    cell += c;
    cell *= BLOCK_SIZE;
    return cell;
}

bool cChunkLayer::outOfBounds( const int & v )const
{
    return ( v >= CHUNK_SIZE || v < 0 );
}

}
}
