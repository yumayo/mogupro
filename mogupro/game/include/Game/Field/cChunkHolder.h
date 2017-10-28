#pragma once
#include <Game/Field/cChunk.h>
#include <Game/Field/cBlock.h>
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

    cChunk& getChunk( const int& x, const int& z );
    cChunk& getChunk( const ci::ivec3& c );
    ChunkMap& getChunks();

    void setChunk( cChunk& chunk );
    void setChunk( const int& x, const int& y, const int& z );

    bool createChunk( cChunk& chunk );
    bool createChunkMesh( cChunk& chunk );
    bool isExistsChunk( const ci::ivec3 & cell );
    bool isExistsChunk( const int& x, const int& y, const int& z );
    bool cellIsOutOfBounds( const int & x, const int & y, const int & z );

    void clear();

private:

    cUnderGround* mUnderGround;
    ChunkMap mChunks;

};
}
}
