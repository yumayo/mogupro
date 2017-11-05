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

bool cChunkHolder::breakBlock( const ci::ivec3 & chunk_cell,
                               const ci::ivec3& block_cell,
                               const float & radius )
{
    if ( isExistsChunk( chunk_cell ) )
        return false;
    if ( cellIsOutOfBounds( block_cell.x, block_cell.y, block_cell.z ) )
        return false;

    auto break_chunk_layer = getChunkLayer( chunk_cell );

    // •K‚¸ˆêƒ}ƒX‚ÍŒ@‚é
    auto first_layer = break_chunk_layer->breakBlock( block_cell );

    if ( radius < 0 )
        return false;

    auto r = ivec3( int( radius / BLOCK_SIZE ) );
    auto s = block_cell - r;
    auto e = block_cell + r;

    // Œ@‚ç‚ê‚½ƒ`ƒƒƒ“ƒN‚ð“o˜^‚·‚é
    std::vector<cChunkLayer*> build_chunk_layers;
    if ( first_layer != nullptr )
        build_chunk_layers.push_back( first_layer );

    for ( int z = s.z; z <= e.z; z++ )
        for ( int y = s.y; y <= e.y; y++ )
            for ( int x = s.x; x <= e.x; x++ )
            {
                auto layer = break_chunk_layer->breakBlock( ivec3( x, y, z ) );
                if ( layer == nullptr )
                    continue;
                if ( std::any_of( build_chunk_layers.begin(), build_chunk_layers.end(),
                                  [&]( cChunkLayer* t ) { return t == layer; } ) )
                    continue;
                build_chunk_layers.push_back( layer );
            }

    // Œ@‚ç‚ê‚½ƒ`ƒƒƒ“ƒN‚ÌŽü‚è‚à“o˜^‚·‚é
    std::vector<cChunkLayer*> temp_layers;
    for ( auto break_layer : build_chunk_layers )
    {
        for ( int i = 0; i < 6; i++ )
        {
            auto cell = break_layer->getChunkCell();
            switch ( i )
            {
                case 0: cell.x += 1; break;
                case 1: cell.x -= 1; break;
                case 2: cell.y += 1; break;
                case 3: cell.y -= 1; break;
                case 4: cell.z += 1; break;
                case 5: cell.z -= 1; break;
            }

            if ( isExistsChunk( cell.x, 0, cell.z ) )
                continue;
            if ( cell.y > getHighestCell() || cell.y < 0 )
                continue;
            if ( std::any_of( build_chunk_layers.begin(), build_chunk_layers.end(),
                              [&]( cChunkLayer* t ) { return t->getChunkCell() == cell; } ) )
                continue;

            auto temp_layer = getChunkLayer( cell );
            temp_layer->mIsBlockBroken = true;
            temp_layers.push_back( temp_layer );
        }
    }

    std::copy( temp_layers.begin(), temp_layers.end(), std::back_inserter( build_chunk_layers ) );

    ivec3 test;
    if ( first_layer != nullptr )
    {
        test = first_layer->getChunkCell();
        test.y = first_layer->getHeight();
        app::console() << "first:" << test << std::endl;
    }

    for ( auto c : build_chunk_layers )
    {
        test = c->getChunkCell();
        test.y = c->getHeight();
        app::console() << test << std::endl;
        c->reBuildStart();
    }
    return true;
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
