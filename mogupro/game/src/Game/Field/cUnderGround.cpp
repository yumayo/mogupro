#include <Game/Field/cUnderGround.h>
#include <Resource/TextureManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Network/cUDPManager.h>
#include <Network/cRequestManager.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{

int cr = 4;

cUnderGround::cUnderGround() :
    mChunkHolder( this )
{
}

cUnderGround::~cUnderGround()
{
    mIsRunning = false;
    for ( auto& thread : mChunkLoadThreads )
    {
        thread.join();
    }
}

void cUnderGround::setup()
{
    TEX->set( "dirt", "dirt.jpg" );

    cTimeMeasurement::getInstance()->make();

    for ( int z = -cr; z < cr; z++ )
    {
        for ( int x = -cr; x < cr; x++ )
        {
            mChunkHolder.setChunk( x, z );
        }
    }
    cTimeMeasurement::getInstance()->make();

    console() << "Chunk set time : " << cTimeMeasurement::getInstance()->deltaTime() << std::endl;


    for ( size_t i = 0; i < 1; i++ )
    {
        mChunkLoadThreads.emplace_back( [&]
        {
            chunkMeshReLoaded();
        }
        );
        mChunkLoadThreads.emplace_back( [&]
        {
            calcChunks();
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
    //while ( mIsRunning )
    //{
    //    for ( int z = -cr; z < cr; z++ )
    //    {
    //        for ( int x = -cr; x < cr; x++ )
    //        {
    //            std::lock_guard<decltype( mMainMutex )> lock( mMainMutex );
    //            auto & chunk = mChunkHolder.getChunk( x, z );
    //            chunk.reBuildMesh();
    //        }
    //    }
    //    mIsRunning = false;
    //}
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
        //mIsRunning = false;
    }
    return true;
}

ci::ivec3 cUnderGround::getChunkCellFromPosition( const ci::vec3 & position )
{
    return ivec3( position ) / CHUNK_SIZE;
}

ci::ivec3 cUnderGround::getBlockCellFromPosition( const ci::vec3 & position )
{
    return ci::ivec3( position ) % CHUNK_SIZE;
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

    if ( mChunkHolder.isExistsChunk( chunk_cell.x, chunk_cell.z ) )
        return false;

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = block_cell - r;
    auto e = block_cell + r;

    for ( int z = s.z; z < e.z; z++ )
        for ( int y = s.y; y < e.y; y++ )
            for ( int x = s.x; x < e.x; x++ )
            {
                mChunkHolder.getChunk( chunk_cell ).breakBlock( ivec3( x, y, z ) );
            }

    return true;
}

ci::vec3 cUnderGround::getBlockCenterTopPosition( const ci::vec3 & target_position )
{
    //auto c = getCellNumFromPosition( target_position );
    //auto b = mBlocks[c.z][c.y][c.x];
    //return b->mPosition + vec3( 0, b->mScale / 2, 0 );
    return vec3();
}

ci::ivec3 cUnderGround::getBlockMaxCell()
{
    return ivec3( CHUNK_SIZE, CHUNK_SIZE, CHUNK_SIZE );
}

}
}
