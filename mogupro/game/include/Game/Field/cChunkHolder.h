#pragma once
#include <Game/Field/cChunk.h>
#include <Game/Field/FieldData.h>
namespace Game
{
namespace Field
{

class cChunkHolder
{
public:

    cChunkHolder();
    ~cChunkHolder();

    cChunk& getChunk( int x, int z );
    cChunk& getChunk( ci::ivec3 c );
    ChunkMap& getChunks();
    void setChunk( cChunk& chunk );

    cChunk createChunk( int x, int z );
    bool isExistsChunks( int x, int z );

private:

    ChunkMap mChunks;

};
}
}
