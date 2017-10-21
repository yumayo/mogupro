#include <Collision/cFallBlockSimple.h>
#include <cinder/gl/gl.h>
namespace Collision
{
cFallBlockSimple::cFallBlockSimple( cinder::vec3 pos )
    : mCollider( pos, cinder::vec3( 0.8F ) )
    , mRigidbody( mCollider )
{
    mCollider.addWorld( );
    mRigidbody.addWorld( );
}
void cFallBlockSimple::draw( )
{
    cinder::gl::drawColorCube( mCollider.getPosition( ), mCollider.getSize( ) );
}
}
