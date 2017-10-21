#include <Game/Field/cUnderGround.h>
#include <Resource/TextureManager.h>
#include <Utility/cTimeMeasurement.h>
#include <Network/cUDPManager.h>
#include <Network/cRequestManager.h>
#include <Game/Field/CalculateTriMesh.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
cUnderGround::cUnderGround()
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

    for ( size_t i = 0; i < 1; i++ )
    {
        mChunkLoadThreads.emplace_back( [&]
        {
            createChunks();
        }
        );
    }
}
void cUnderGround::update()
{
    //mMainMutex.lock();

    ChunkMap& chunks = mChunkHolder.getChunks();
    for ( auto& chunk : chunks )
        chunk.second.createMainCall();

    //mMainMutex.unlock();
}
void cUnderGround::draw()
{
    //auto texture = TEX.get( "dirt" );
    //if (!texture)
    //    return;
    //using namespace ci::gl;

    //auto ctx = context();

    //Rectf texRect = texture->getAreaTexCoords( Area( vec2( 0 ), texture->getSize() ) );

    //ScopedVao vaoScp( ctx->getDrawTextureVao() );
    //ScopedBuffer vboScp( ctx->getDrawTextureVbo() );
    //ScopedTextureBind texBindScope( texture );

    //auto glsl = getStockShader( ShaderDef().uniformBasedPosAndTexCoord().color().texture( texture ) );
    //ScopedGlslProg glslScp( glsl );
    //glsl->uniform( "uTex0", 0 );
    //glsl->uniform( "uPositionOffset", vec2( 0 ) );
    //glsl->uniform( "uPositionScale", vec2( 1 ) );
    //glsl->uniform( "uTexCoordOffset", texRect.getUpperLeft() );
    //glsl->uniform( "uTexCoordScale", texRect.getSize() );

    //mMainMutex.lock();

    ChunkMap& chunks = mChunkHolder.getChunks();
    for ( auto& chunk : chunks )
        chunk.second.draw();

    //mMainMutex.unlock();

}
bool cUnderGround::createChunks()
{
    while ( mIsRunning )
    {
        for ( size_t z = 0; z < 100; z++ )
        {
            for ( size_t x = 0; x < 100; x++ )
            {
                mChunkHolder.createChunk( x, z );
            }
        }
        mIsRunning = false;
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
bool cUnderGround::blockBreak( const ci::vec3& position, const float& radius )
{
    auto chunk_cell = getChunkCellFromPosition( position );
    auto block_cell = getBlockCellFromPosition( position );

    if ( mChunkHolder.isExistsChunks( chunk_cell.x, chunk_cell.z ) )
        return false;
    auto& chunks = mChunkHolder.getChunk( chunk_cell );

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = block_cell - r;
    auto e = block_cell + r;

    for ( int z = s.z; z < e.z; z++ )
        for ( int y = s.y; y < e.y; y++ )
            for ( int x = s.x; x < e.x; x++ )
                chunks.breakBlock( ivec3( x, y, z ) );

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
