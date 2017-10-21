#include <Game/Field/cChunkHolder.h>
#include <Game/Field/CalculateTriMesh.h>
#include <Utility/cString.h>
using namespace ci;
namespace Game
{
namespace Field
{
cChunkHolder::cChunkHolder()
{

}
cChunkHolder::~cChunkHolder()
{

}
cChunk & cChunkHolder::getChunk( int x, int z )
{
    return getChunk( ci::ivec3( x, 0, z ) );
}
cChunk & cChunkHolder::getChunk( ci::ivec3 c )
{
    if ( isExistsChunk( c.x, c.z ) )
        return cChunk();
    return mChunks[c];
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
void cChunkHolder::setChunk( const int& x, const int& z )
{
    if ( isExistsChunk( x, z ) == false )
        return;
    mChunks[ivec3( x, 0, z )] = cChunk( x, z );
}
bool cChunkHolder::createChunk( const int& x, const int& z )
{
    if ( isExistsChunk( x, z ) )
        return false;
    auto & chunk = mChunks[ivec3( x, 0, z )];
    if ( chunk.mIsLoaded )
        return false;
    chunk.mIsLoaded = true;
    chunk.createBlocks();
    chunk = calcChunkData( chunk );
    chunk.mIsDone = true;

    return true;
}
bool cChunkHolder::isExistsChunk( const int& x, const int& z )
{
    return mChunks.find( ivec3( x, 0, z ) ) == mChunks.end();
}
}
}
