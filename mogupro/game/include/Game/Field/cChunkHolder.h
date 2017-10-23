#pragma once
#include <Game/Field/cChunk.h>
#include <Game/Field/FieldData.h>
namespace Game
{
namespace Field
{
class cUnderGround;
class cChunkHolder
{
public:

    cChunkHolder( cUnderGround* under_ground );
    ~cChunkHolder();

    cChunk& getChunk( int x, int z );
    cChunk& getChunk( ci::ivec3 c );
    ChunkMap& getChunks();

    void setChunk( cChunk& chunk );
    void setChunk( const int& x, const int& z );

    bool createChunk( cChunk& chunk );
    bool isExistsChunk( const int& x, const int& z );

private:

    cUnderGround* mUnderGround;
    ChunkMap mChunks;

};
}
}
