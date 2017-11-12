#include <Game/Field/cChunk.h>
#include <Game/Field/cUnderGround.h>

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
}

cChunk::cChunk( int x, int z, cUnderGround* under_ground ) :
    mChunkCell( ci::ivec3( x, 0, z ) )
    , mUnderGround( under_ground )
{
    setup();
}

cChunk::~cChunk()
{
}

void cChunk::setup()
{
    for ( int y = 0; y < CHUNK_RANGE_Y + 1; y++ )
    {
        mChunkLayers.push_back( cChunkLayer( y, this, mUnderGround ) );
        // ˆê”Ôã‚ÌŠK‘w‚ÍƒuƒƒbƒN‚ð’u‚©‚È‚¢
        if ( y == CHUNK_RANGE_Y )
            mChunkLayers.back().mIsActive = false;
    }
}

void cChunk::update()
{
    for ( auto& layer : mChunkLayers )
        layer.update();
}

void cChunk::draw()
{
    for ( auto& layer : mChunkLayers )
        layer.draw();
}

ci::ivec3 cChunk::getCell()
{
    return mChunkCell;
}

cBlock* cChunk::getBlock( const int& x, const int& y, const int& z )
{
    return getBlock( ci::ivec3( x, y, z ) );
}

cBlock* cChunk::getBlock( const ci::ivec3& cell )
{
    int y = cell.y / CHUNK_SIZE;
    if ( y >= (int) mChunkLayers.size() || y < 0 )
        return nullptr;
    auto& layer = mChunkLayers[y];
    y = cell.y % CHUNK_SIZE;
    return layer.getBlock( cell.x, y, cell.z );
}

void cChunk::setBlock( const ci::ivec3 & cell, cBlock * block )
{
    int y = cell.y / CHUNK_SIZE;
    if ( y >= (int) mChunkLayers.size() || y < 0 )
        return;
    auto& layer = mChunkLayers[y];
    y = cell.y % CHUNK_SIZE;
    layer.setBlock( cell.x, y, cell.z, block );
}

cChunkLayer* cChunk::getChunkLayer( const int & height )
{
    if ( height >= (int) mChunkLayers.size() )
        return &mChunkLayers[mChunkLayers.size() - 1];
    return &mChunkLayers[height];
}

void cChunk::reBuildStart()
{
    for ( auto& layer : mChunkLayers )
        layer.reBuildStart();
}

void cChunk::reBuildMesh()
{
    for ( auto& layer : mChunkLayers )
        layer.reBuildMesh();
}

void cChunk::buildMesh()
{
    for ( auto& layer : mChunkLayers )
        layer.buildMesh();
}

bool cChunk::reLoading()
{
    bool is_reloading = false;
    for ( auto& layer : mChunkLayers )
    {
        if ( is_reloading == false )
            is_reloading = layer.reLoading();
        else
            layer.reLoading();
    }
    return is_reloading;
}

bool cChunk::createMainCall()
{
    for ( auto& layer : mChunkLayers )
        layer.createMainCall();
    return true;
}

void cChunk::createBlocks()
{
    for ( auto& layer : mChunkLayers )
    {
        if ( layer.mIsLoaded )
            continue;
        layer.mIsLoaded = true;
        layer.createBlocks();
        layer.mIsDone = true;
    }
}

bool cChunk::isOutOfRange( ci::ivec3 c )
{
    if ( c.x >= CHUNK_SIZE || c.z >= CHUNK_SIZE )
        return true;
    if ( c.x < 0 || c.y < 0 || c.z < 0 )
        return true;
    if ( c.y >= CHUNK_RANGE_Y  * CHUNK_SIZE )
        return true;
    return false;
}
}
}
