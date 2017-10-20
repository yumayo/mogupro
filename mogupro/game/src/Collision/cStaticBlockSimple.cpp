#include <Collision/cStaticBlockSimple.h>
#include <cinder/gl/gl.h>
namespace Collision
{
cStaticBlockSimple::cStaticBlockSimple( cinder::vec3 pos, cinder::vec3 size )
    : mCollider( pos, size )
{
    mCollider.addWorld( );
}
void cStaticBlockSimple::draw( )
{
    cinder::gl::drawColorCube( mCollider.getPosition( ), mCollider.getSize( ) );
}
}
