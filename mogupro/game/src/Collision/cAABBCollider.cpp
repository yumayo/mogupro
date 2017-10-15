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
    cCollisionManager::getInstance( )->add( *this );
}
cAABBCollider::~cAABBCollider( )
{
    cCollisionManager::getInstance( )->remove( *this );
}
cinder::AxisAlignedBox cAABBCollider::createAABB( cinder::vec3 calcedPosition ) const
{
    cinder::AxisAlignedBox aabb( -mSize * mAnchor, mSize * mAnchor );
    aabb.transform( translate( cinder::mat4( ), calcedPosition ) );
    return aabb;
}
}
