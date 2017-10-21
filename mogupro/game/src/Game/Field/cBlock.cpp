#include <Game/Field/cBlock.h>
#include <cinder/gl/gl.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
cBlock::cBlock() :
    mPosition( vec3( 0 ) ),
    mScale( BLOCK_SIZE ),
    mIsActive( false ),
    mCollider( mPosition, vec3( BLOCK_SIZE ), vec3( 0.5f ) )
{

}
cBlock::cBlock( const ci::vec3& position, const float& scale ) :
    mPosition( position ),
    mScale( scale ),
    mIsActive( true ),
    mCollider( mPosition, vec3( scale ), vec3( 0.5f ) )
{
}
cBlock::~cBlock()
{

}
void cBlock::setup()
{
    mCollider.addWorld();
}
void cBlock::calcMeshIndexData( uint num )
{
    mId = num;
    for ( uint i = 0; i < cube_vertices_index_size; i++ )
    {
        uint n = i + mId * cube_vertices_index_size;
        mVerticesNum.emplace_back( n );
        mNormalsNum.emplace_back( n );
        mUvNum.emplace_back( n );
    }
    for ( uint i = 0; i < cube_indices_index_size; i++ )
        mIndicesNum.emplace_back( i + mId * cube_indices_index_size );
}
void cBlock::clear()
{
}
void cBlock::toBreak()
{
    mIsActive = false;
    clear();
    mCollider.removeWorld();
}
}
}