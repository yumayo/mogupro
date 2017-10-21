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
    if ( isExistsChunks( c.x, c.z ) )
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
bool cChunkHolder::createChunk( int x, int z )
{
    if ( isExistsChunks( x, z ) == false )
        return false;
    cChunk chunk = cChunk( x, z );
    chunk.createBlocks();
    chunk = calcChunkData( chunk );
    chunk.mIsDone = true;
    setChunk( chunk );

    return true;
}
bool cChunkHolder::isExistsChunks( int x, int z )
{
    return mChunks.find( ivec3( x, 0, z ) ) == mChunks.end();
}
}
}
