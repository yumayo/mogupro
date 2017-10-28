#include <Game/Field/cChunkHolder.h>
#include <Game/Field/cUnderGround.h>
#include <Game/Field/cChunkMeshBuilder.h>
#include <Utility/cString.h>

using namespace ci;
namespace Game
{
namespace Field
{
cChunkHolder::cChunkHolder( cUnderGround* under_ground ) :
    mUnderGround( under_ground )
{
}

cChunkHolder::~cChunkHolder()
{

}

cChunk & cChunkHolder::getChunk( const int& x, const int& z )
{
    return getChunk( ci::ivec3( x, 0, z ) );
}

cChunk & cChunkHolder::getChunk( const ci::ivec3& c )
{
    return mChunks.at( c );
}

ChunkMap& cChunkHolder::getChunks()
{
    return mChunks;
}

void cChunkHolder::setChunk( cChunk&  chunk )
{
    auto cell = chunk.getCell();
    mChunks[cell] = chunk;
}

void cChunkHolder::setChunk( const int& x, const int& y, const int& z )
{
    if ( isExistsChunk( x, y, z ) == false )
        return;
    mChunks[ivec3( x, y, z )] = cChunk( x, z, mUnderGround );
}

bool cChunkHolder::createChunk( cChunk& chunk )
{
    if ( chunk.mIsLoaded )
        return false;
    chunk.mIsLoaded = true;
    chunk.createBlocks();
    return chunk.mIsDone = true;
}

bool cChunkHolder::createChunkMesh( cChunk & chunk )
{
    chunk.buildMesh();
    return true;
}

bool cChunkHolder::isExistsChunk( const ci::ivec3 & cell )
{
    return mChunks.find( cell ) == mChunks.end();
}

bool cChunkHolder::isExistsChunk( const int& x, const int& y, const int& z )
{
    return isExistsChunk( ivec3( x, y, z ) );
}

bool cChunkHolder::cellIsOutOfBounds( const int & x, const int & y, const int & z )
{
    if ( x >= CHUNK_SIZE || y >= CHUNK_SIZE || z >= CHUNK_SIZE )
        return true;
    if ( x < 0 || y < 0 || z < 0 )
        return true;
    return false;
}

void cChunkHolder::clear()
{
    mChunks.clear();
}
}
}
