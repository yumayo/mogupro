#include <Game/Field/cChunkHolder.h>
#include <Game/Field/CalculateTriMesh.h>
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
        return createChunk( c.x, c.z );
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
cChunk cChunkHolder::createChunk( int x, int z )
{
    cChunk chunk = cChunk( x, z );
    chunk.createBlocks();
    chunk = calcChunkData( chunk );
    return chunk;
}
bool cChunkHolder::isExistsChunks( int x, int z )
{
    return mChunks.find( ivec3( x, 0, z ) ) == mChunks.end();
}
}
}
