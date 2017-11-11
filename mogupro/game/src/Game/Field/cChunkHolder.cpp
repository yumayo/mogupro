#include <Game/Field/cChunkHolder.h>
#include <Game/Field/cUnderGround.h>
#include <Game/Field/cChunkMeshBuilder.h>
#include <Utility/cString.h>

using namespace ci;
using namespace ci::app;
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

cChunk* cChunkHolder::getChunk( const int& x, const int& z )
{
    return getChunk( ci::ivec3( x, 0, z ) );
}

cChunk* cChunkHolder::getChunk( const ci::ivec3& c )
{
    return mChunks.at( ivec3( c.x, 0, c.z ) ).get();
}

cChunkLayer* cChunkHolder::getChunkLayer( const ci::ivec3 & cell )
{
    return getChunk( ivec3( cell.x, 0, cell.z ) )->getChunkLayer( cell.y );
}

ChunkMap& cChunkHolder::getChunks()
{
    return mChunks;
}

void cChunkHolder::setChunk( cChunkRef&  chunk )
{
    auto cell = chunk->getCell();
    mChunks[cell] = chunk;
}

void cChunkHolder::setChunk( const int& x, const int& y, const int& z )
{
    if ( isExistsChunk( x, y, z ) == false )
        return;
    mChunks[ivec3( x, y, z )] = std::make_shared<cChunk>( x, z, mUnderGround );
}

bool isPointToSphere( const ci::vec3& point, const ci::vec3& sphere_pos, const float& radius )
{
    float x = ( sphere_pos.x - point.x ) *( sphere_pos.x - point.x );
    float y = ( sphere_pos.y - point.y ) *( sphere_pos.y - point.y );
    float z = ( sphere_pos.z - point.z ) *( sphere_pos.z - point.z );
    return ( x + y + z ) <= radius * radius;
}

bool cChunkHolder::breakBlock( const ci::ivec3 & chunk_cell,
                               const ci::ivec3& block_cell,
                               const ci::vec3& sphere_pos,
                               const float & radius,
                               const cBreakBlockType& type )
{
    if ( isExistsChunk( chunk_cell ) )
        return false;
    if ( cellIsOutOfBounds( block_cell.x, block_cell.y, block_cell.z ) )
        return false;


    if ( radius < 0 )
        return false;

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = block_cell - r;
    auto e = block_cell + r;

    auto break_chunk_layer = getChunkLayer( chunk_cell );
    for ( int z = s.z; z <= e.z; z++ )
        for ( int y = s.y; y <= e.y; y++ )
            for ( int x = s.x; x <= e.x; x++ )
            {
                auto block = break_chunk_layer->getBlock( ivec3( x, y, z ) );
                if ( block == nullptr )
                    continue;
                if ( block->isActive() == false )
                    continue;
                if ( isPointToSphere( block->getPosition(), sphere_pos, radius ) == false )
                    continue;

                return true;
            }

    return false;
}

bool cChunkHolder::isBreakBlock( const ci::ivec3 & chunk_cell,
                                 const ci::ivec3 & block_cell,
                                 const ci::vec3 & sphere_pos,
                                 const float & radius )
{


    return false;
}

bool cChunkHolder::createChunk( cChunk* chunk )
{
    chunk->createBlocks();
    return true;
}

bool cChunkHolder::createChunkMesh( cChunk* chunk )
{
    chunk->buildMesh();
    return true;
}

bool cChunkHolder::isExistsChunk( const ci::ivec3 & cell )
{
    return mChunks.find( ivec3( cell.x, 0, cell.z ) ) == mChunks.end();
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

int cChunkHolder::getHighestCell()
{
    return CHUNK_RANGE_Y;
}

void cChunkHolder::clear()
{
    mChunks.clear();
}
}
}
