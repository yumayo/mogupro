#include <Collision/cFallBlockSimple.h>
#include <cinder/gl/gl.h>
namespace Collision
{
cFallBlockSimple::cFallBlockSimple( cinder::vec3 pos )
    : mCollider( std::make_shared<Collision::cAABBCollider>( pos, cinder::vec3( 0.8F ) ) )
    , mRigidbody( std::make_shared<Collision::cRigidBody>( *mCollider ) )
{

}
void cFallBlockSimple::draw( )
{
    cinder::gl::drawColorCube( mCollider->getPosition( ), mCollider->getSize( ) );
}
}
