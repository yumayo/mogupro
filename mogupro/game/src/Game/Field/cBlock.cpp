#include <Game/Field/cBlock.h>
#include <cinder/gl/gl.h>

using namespace ci;
using namespace ci::app;

namespace Game
{
namespace Field
{
cBlock::cBlock() :
    mPosition( vec3( 0 ) )
    , mScale( BLOCK_SIZE )
    , mIsActive( false )
    , mCollider( mPosition, vec3( BLOCK_SIZE ), vec3( 0.5f ) )
{

}
cBlock::cBlock( const ci::vec3& position, const float& scale, const uint & id ) :
    mPosition( position )
    , mScale( scale )
    , mId( id )
    , mIsActive( true )
    , mCollider( mPosition, vec3( scale ), vec3( 0.5f ) )
{
}
cBlock::~cBlock()
{

}
void cBlock::setup()
{
    mCollider.addWorld();
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