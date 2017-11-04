#include <Game/Field/cUnderGround.h>
#include <Resource/TextureManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Network/cUDPManager.h>
#include <Network/cRequestManager.h>
#include <Utility/cString.h>
#include <Game/cClientAdapter.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
cBlock none_block;
cChunk none_chunk;

cUnderGround::cUnderGround() :
    mChunkHolder( this )
{
}

cUnderGround::~cUnderGround()
{
}

void cUnderGround::setup()
{
    TEX->set( "dirt", "dirt.jpg" );

    for ( int z = 0; z < CHUNK_RANGE_Z; z++ )
        for ( int x = 0; x < CHUNK_RANGE_X; x++ )
            mChunkHolder.setChunk( x, 0, z );

    for ( size_t i = 0; i < 1; i++ )
    {
        mChunkLoadThreads.emplace_back( [&]
        {
            calcChunks();
        }
        );
        mChunkLoadThreads.emplace_back( [&]
        {
            chunkMeshReLoaded();
        }
        );
    }
}

void cUnderGround::update()
{
    std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );

    ChunkMap& chunks = mChunkHolder.getChunks();
    for ( auto& chunk : chunks )
    {
        chunk.second->update();
        chunk.second->createMainCall();
    }
}

void cUnderGround::draw()
{
    std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );

    auto texture = TEX->get( "dirt" );
    if ( !texture )
        return;

    using namespace ci::gl;
    auto ctx = context();

    Rectf texRect = texture->getAreaTexCoords( Area( vec2( 0 ), texture->getSize() ) );

    ScopedVao vaoScp( ctx->getDrawTextureVao() );
    ScopedBuffer vboScp( ctx->getDrawTextureVbo() );
    ScopedTextureBind texBindScope( texture );
    //ci::gl::ScopedGlslProg glsl( ci::gl::getStockShader( ci::gl::ShaderDef().texture() ) );

    //auto glsl = getStockShader( ShaderDef().uniformBasedPosAndTexCoord().color().texture( texture ) );
    //ScopedGlslProg glslScp( glsl );
    //glsl->uniform( "uTex0", 0 );
    //glsl->uniform( "uPositionOffset", vec2( 0 ) );
    //glsl->uniform( "uPositionScale", vec2( 1 ) );
    //glsl->uniform( "uTexCoordOffset", texRect.getUpperLeft() );
    //glsl->uniform( "uTexCoordScale", texRect.getSize() );

    ChunkMap& chunks = mChunkHolder.getChunks();
    for ( auto& chunk : chunks )
        chunk.second->draw();
}

void cUnderGround::shutdown()
{
    mIsRunning = false;
    for ( auto& thread : mChunkLoadThreads )
    {
        thread.join();
    }
    mChunkHolder.clear();
}

bool cUnderGround::chunkMeshReLoaded()
{
    while ( mIsRunning )
    {
        for ( int z = 0; z < CHUNK_RANGE_Z; z++ )
        {
            for ( int x = 0; x < CHUNK_RANGE_X; x++ )
            {
                std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
                auto chunk = mChunkHolder.getChunk( x, z );
                chunk->reBuildMesh();
            }
        }
    }

    return true;
}

bool cUnderGround::calcChunks()
{
    while ( mIsRunning )
    {
        for ( int z = 0; z < CHUNK_RANGE_Z; z++ )
        {
            for ( int x = 0; x < CHUNK_RANGE_X; x++ )
            {
                std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
                auto chunk = mChunkHolder.getChunk( x, z );
                mChunkHolder.createChunk( chunk );
            }
        }
        break;
    }

    while ( mIsRunning )
    {
        for ( int z = 0; z < CHUNK_RANGE_Z; z++ )
        {
            for ( int x = 0; x < CHUNK_RANGE_X; x++ )
            {
                std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
                auto chunk = mChunkHolder.getChunk( x, z );
                mChunkHolder.createChunkMesh( chunk );
            }
        }
        return true;
    }
    return true;
}

ci::ivec3 cUnderGround::getChunkCellFromPosition( const ci::vec3 & position )
{
    // マップチップ番号に直す
    ivec3 pos = position / BLOCK_SIZE;
    return pos / CHUNK_SIZE;
}

ci::ivec3 cUnderGround::getBlockCellFromPosition( const ci::vec3 & position )
{
    // マップチップ番号に直す
    ivec3 pos = position / BLOCK_SIZE;
    return pos % CHUNK_SIZE;
}

cBlock* cUnderGround::getBlock( const ci::vec3& position )
{
    auto chunk_cell = getChunkCellFromPosition( position );
    auto block_cell = getBlockCellFromPosition( position );

    if ( mChunkHolder.isExistsChunk( chunk_cell ) )
        return &none_block;
    if ( mChunkHolder.cellIsOutOfBounds( block_cell.x, block_cell.y, block_cell.z ) )
        return &none_block;

    auto height = ivec3( 0, chunk_cell.y * CHUNK_SIZE, 0 );
    if( (height.y / CHUNK_SIZE) > CHUNK_RANGE_Y )
        return &none_block;

    return mChunkHolder.getChunk( chunk_cell )->getBlock( block_cell + height );
}

bool cUnderGround::blockBreak( const ci::vec3& position, const float& radius )
{
    cClientAdapter::getInstance()->sendBreakBlock( position, radius );
    return true;
}

bool cUnderGround::blockBreakNetwork( const ci::vec3 & position, const float & radius )
{
    auto chunk_cell = getChunkCellFromPosition( position );
    auto block_cell = getBlockCellFromPosition( position );
    return mChunkHolder.breakBlock( chunk_cell, block_cell, radius );
}

ci::vec3 cUnderGround::getBlockTopPosition( const ci::vec3 & target_position )
{
    auto chunk_cell = getChunkCellFromPosition( target_position );
    vec3 block_cell = getBlockCellFromPosition( target_position );
    chunk_cell *= ( CHUNK_SIZE * BLOCK_SIZE );
    chunk_cell.y = 0;
    block_cell.y = CHUNK_RANGE_Y * CHUNK_SIZE * BLOCK_SIZE;
    block_cell += chunk_cell;
    block_cell.y += BLOCK_SIZE / 2.0f + 0.1f;
    return block_cell;
}

ci::vec3 cUnderGround::getBlockHighestPosition( const ci::vec3 & target_position )
{
    auto chunk_cell = getChunkCellFromPosition( target_position );
    vec3 block_cell = getBlockCellFromPosition( target_position );
    chunk_cell *= ( CHUNK_SIZE * BLOCK_SIZE );
    chunk_cell.y = 0;
    block_cell.y = CHUNK_RANGE_Y * CHUNK_SIZE * BLOCK_SIZE;
    block_cell += chunk_cell;
    block_cell.y += BLOCK_SIZE / 2.0f;
    return block_cell;
}

ci::ivec3 cUnderGround::getBlockMaxCell()
{
    return ivec3( CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE );
}

}
}
