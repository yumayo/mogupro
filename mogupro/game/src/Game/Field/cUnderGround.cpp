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

    std::this_thread::sleep_for( std::chrono::milliseconds( 50 ) );
    //mChunkLoadThreads.emplace_back( [&]
    //{
    createUnderGround();
    //});

}
void cUnderGround::update()
{
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

    mMainMutex.lock();
    ChunkMap& chunks = mChunkHolder.getChunks();
    for ( auto& chunk : chunks )
        chunk.second.draw();
    mMainMutex.unlock();

}
bool cUnderGround::createUnderGround()
{
    //while ( mIsRunning )
    {
        for ( size_t x = 0; x < 4; x++ )
        {
            for ( size_t z = 0; z < 4; z++ )
            {
                std::this_thread::sleep_for( std::chrono::milliseconds( 1 ) );

                if ( mChunkHolder.isExistsChunks( x, z ) == false )
                    continue;
                mMainMutex.lock();
                mChunkHolder.createChunk( x, z );
                mMainMutex.unlock();
            }
        }
    }
    return true;
}
bool cUnderGround::createMesh()
{
    return true;
}
bool cUnderGround::loadChunks()
{
    return false;
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
    auto& chunks = mChunkHolder.getChunk( chunk_cell );

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = chunk_cell - r;
    auto e = chunk_cell + r;

    for ( int z = s.z; z < e.z; z++ )
        for ( int y = s.y; y < e.y; y++ )
            for ( int x = s.x; x < e.x; x++ )
                chunks.breakBlock( block_cell );

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
    //return mBlockMaxCell;
    return ivec3();
}
}
}
