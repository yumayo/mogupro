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

    cChunk* getChunk( const int& x, const int& z );
    cChunk* getChunk( const ci::ivec3& c );
    cChunkLayer* getChunkLayer( const ci::ivec3& cell );
    ChunkMap& getChunks();

    void setChunk( cChunkRef& chunk );
    void setChunk( const int& x, const int& y, const int& z );

    bool breakBlock( const ci::ivec3& chunk_cell,
                     const ci::ivec3& block_cell_,
                     const ci::vec3& sphere_pos,
                     const float & radius,
                     const cBreakBlockType& type );
    bool createChunk( cChunk* chunk );
    bool createChunkMesh( cChunk* chunk );
    bool isExistsChunk( const ci::ivec3 & cell );
    bool isExistsChunk( const int& x, const int& y, const int& z );
    bool cellIsOutOfBounds( const int & x, const int & y, const int & z );
    int getHighestCell();

    void clear();

private:

    cUnderGround* mUnderGround;
    ChunkMap mChunks;

};
}
}
