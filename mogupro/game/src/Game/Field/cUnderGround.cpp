#include <Game/Field/cUnderGround.h>
#include <Resource/TextureManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Network/cUDPManager.h>
#include <Network/cRequestManager.h>
#include <Utility/cString.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{


cUnderGround::cUnderGround() :
    mChunkHolder( this )
{
}

cUnderGround::~cUnderGround()
{
    std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
    mChunkHolder.clear();
    mIsRunning = false;
    for ( auto& thread : mChunkLoadThreads )
    {
        thread.join();
    }
}

int cr = 2;

void cUnderGround::setup()
{
    TEX->set( "dirt", "dirt.jpg" );

    cTimeMeasurement::getInstance()->make();

    for ( int z = -cr; z < cr; z++ )
    {
        for ( int x = -cr; x < cr; x++ )
        {
            mChunkHolder.setChunk( x, 0, z );
        }
    }
    cTimeMeasurement::getInstance()->make();

    //console() << "Chunk set time : " << cTimeMeasurement::getInstance()->deltaTime() << std::endl;

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
        chunk.second.update();
        chunk.second.createMainCall();
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

    auto glsl = getStockShader( ShaderDef().uniformBasedPosAndTexCoord().color().texture( texture ) );
    ScopedGlslProg glslScp( glsl );
    glsl->uniform( "uTex0", 0 );
    glsl->uniform( "uPositionOffset", vec2( 0 ) );
    glsl->uniform( "uPositionScale", vec2( 1 ) );
    glsl->uniform( "uTexCoordOffset", texRect.getUpperLeft() );
    glsl->uniform( "uTexCoordScale", texRect.getSize() );

    ChunkMap& chunks = mChunkHolder.getChunks();
    for ( auto& chunk : chunks )
        chunk.second.draw();
}

bool cUnderGround::chunkMeshReLoaded()
{
    while ( mIsRunning )
    {
        for ( int z = -cr; z < cr; z++ )
        {
            for ( int x = -cr; x < cr; x++ )
            {
                std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
                auto & chunk = mChunkHolder.getChunk( x, z );
                chunk.reBuildMesh();
            }
        }
    }

    return true;
}

bool cUnderGround::calcChunks()
{
    while ( mIsRunning )
    {
        for ( int z = -cr; z < cr; z++ )
        {
            for ( int x = -cr; x < cr; x++ )
            {
                std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
                auto & chunk = mChunkHolder.getChunk( x, z );
                mChunkHolder.createChunk( chunk );
            }
        }
        return true;
    }
    return true;
}

ci::ivec3 cUnderGround::getChunkCellFromPosition( const ci::vec3 & position )
{
    vec3 abs_p = vec3( std::abs( position.x ), std::abs( position.y ), std::abs( position.z ) );
    ivec3 cell = ( ivec3( abs_p ) / CHUNK_SIZE );
    if ( position.x < 0 )
    {
        cell.x *= -1;
        cell.x -= 1;
    }
    if ( position.y < 0 )
    {
        cell.y *= -1;
        cell.y -= 1;
    }
    if ( position.z < 0 )
    {
        cell.z *= -1;
        cell.z -= 1;
    }
    return cell;
}

ci::ivec3 cUnderGround::getBlockCellFromPosition( const ci::vec3 & position )
{
    ivec3 c = ivec3( position ) % CHUNK_SIZE;
    if ( c.x < 0 )
        c.x += CHUNK_SIZE;
    if ( c.z < 0 )
        c.z += CHUNK_SIZE;
    return ivec3( std::abs( c.x ), std::abs( c.y ), std::abs( c.z ) );
}

cBlock cUnderGround::getBlock( ci::ivec3 position )
{
    auto block_cell = getBlockCellFromPosition( position );
    auto chunk_cell = getChunkCellFromPosition( position );

    return mChunkHolder.getChunk( chunk_cell ).getBlock( block_cell );
}

void cUnderGround::setBlock( ci::ivec3 position, cBlock block )
{
    if ( position.y <= 0 )
        return;
    auto block_cell = getBlockCellFromPosition( position );
    auto chunk_cell = getChunkCellFromPosition( position );

    mChunkHolder.getChunk( chunk_cell ).setBlock( block_cell, block );
}

bool cUnderGround::blockBreak( const ci::vec3& position, const float& radius )
{
    auto chunk_cell = getChunkCellFromPosition( position );
    auto block_cell = getBlockCellFromPosition( position );

    if ( mChunkHolder.isExistsChunk( chunk_cell.x, chunk_cell.y, chunk_cell.z ) )
        return false;

    auto & chunk = mChunkHolder.getChunk( chunk_cell );

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = block_cell - r;
    auto e = block_cell + r;

    for ( int z = s.z; z < e.z; z++ )
        for ( int y = s.y; y < e.y; y++ )
            for ( int x = s.x; x < e.x; x++ )
            {
                chunk.breakBlock( ivec3( x, y, z ) );
            }

    // ここでbreakblockが呼ばれたチャンクを再構成するフラグを立てる
    chunk.reBuildStart();

    return true;
}

ci::vec3 cUnderGround::getBlockTopPosition( const ci::vec3 & target_position )
{
    auto center_top = getBlockHighestPosition( target_position );
    center_top.y = CHUNK_SIZE + ( BLOCK_SIZE / 2.0f );
    return center_top;
}

ci::vec3 cUnderGround::getBlockHighestPosition( const ci::vec3 & target_position )
{
    auto chunk_cell = getChunkCellFromPosition( target_position );
    auto block_cell = getBlockCellFromPosition( target_position );
    block_cell.y = CHUNK_SIZE - 1;

    if ( mChunkHolder.isExistsChunk( chunk_cell.x, chunk_cell.y, chunk_cell.z ) )
        return ivec3( 0 );

    auto & chunk = mChunkHolder.getChunk( chunk_cell );
    auto block_pos = chunk.getBlock( block_cell ).mPosition;
    block_pos.y += ( BLOCK_SIZE / 2.0f );
    return block_pos;
}

ci::ivec3 cUnderGround::getBlockMaxCell()
{
    return ivec3( CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE );
}

}
}
