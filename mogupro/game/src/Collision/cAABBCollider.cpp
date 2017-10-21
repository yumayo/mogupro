#include <Collision/cAABBCollider.h>
#include <cinder/Matrix44.h>
#include <Collision/cCollisionManager.h>
namespace Collision
{
cAABBCollider::cAABBCollider( cinder::vec3 position, cinder::vec3 size, cinder::vec3 anchor )
    : cColliderBase( Type::AABB, position )
    , mSize( size )
    , mAnchor( anchor )
{
}
cAABBCollider::~cAABBCollider( )
{
}
void cAABBCollider::addWorld( )
{
    cCollisionManager::getInstance( )->add( *this );
}
void cAABBCollider::removeWorld( )
{
    cCollisionManager::getInstance( )->remove( *this );
}
cinder::AxisAlignedBox cAABBCollider::createAABB( cinder::vec3 calcedPosition ) const
{
    cinder::AxisAlignedBox aabb( -mSize * mAnchor, mSize * ( 1.0F - mAnchor ) );
    aabb.transform( translate( cinder::mat4( ), calcedPosition ) );
    return aabb;
}
cinder::vec3 const & cAABBCollider::getSize( )
{
    return mSize;
}
cinder::vec3 const & cAABBCollider::getAnchor( )
{
    return mAnchor;
}
}
