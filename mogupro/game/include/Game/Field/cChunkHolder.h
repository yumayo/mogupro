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

    void setChunk( const int& x, const int& z );
    bool createChunk( const int& x, const int& z );
    bool isExistsChunk( const int& x, const int& z );

private:

    ChunkMap mChunks;

};
}
}
