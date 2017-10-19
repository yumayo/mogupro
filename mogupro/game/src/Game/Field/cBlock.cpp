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
    mScale( 1 ),
    mIsActive( true ),
    mNum( 0 ),
    mCollider( mPosition, vec3( mScale ), vec3( 0.5f ) )
{
}
cBlock::cBlock( const ci::vec3& position, const float& scale, const uint& num ) :
    mPosition( position ),
    mScale( scale ),
    mIsActive( true ),
    mNum( num ),
    mCollider( mPosition, vec3( scale ), vec3( 0.5f ) )
{
}
cBlock::~cBlock()
{
}
void cBlock::update()
{
}
void cBlock::draw()
{
    if ( mIsActive == false ) return;
    gl::pushModelView();
    gl::translate( mPosition );
    gl::scale( vec3( mScale ) );
    gl::drawCube( vec3( 0 ), vec3( 1 ) );
    gl::popModelView();
}
void cBlock::calcMeshIndexData( uint num )
{
    for ( uint i = 0; i < cube_vertices_index_size; i++ )
    {
        uint n = i + num * cube_vertices_index_size;
        mVerticesNum.emplace_back( n );
        mNormalsNum.emplace_back( n );
        mUvNum.emplace_back( n );
    }

    for ( uint i = 0; i < cube_indices_index_size; i++ )
        mIndicesNum.emplace_back( i + num * cube_indices_index_size );
}
void cBlock::clear()
{
}
void cBlock::toBreak()
{
    mIsActive = false;
    clear();

}
}
}