#include <Collision/cAABBCollider.h>
#include <cinder/Matrix44.h>
#include <Collision/cCollisionManager.h>
namespace Collision
{
cAABBCollider::cAABBCollider( )
	: cColliderBase( Type::AABB, cinder::vec3( 0 ) )
	, mSize( cinder::vec3( 0 ) )
{
}
cAABBCollider::cAABBCollider( cinder::vec3 position, cinder::vec3 size )
    : cColliderBase( Type::AABB, position )
    , mSize( size )
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
    cinder::AxisAlignedBox aabb( -mSize / 2.0F, mSize / 2.0F );
    aabb.transform( translate( cinder::mat4( ), calcedPosition ) );
    return aabb;
}
void cAABBCollider::setSize( cinder::vec3 const & value )
{
	cCollisionManager::getInstance( )->remove( *this );
	mSize = value;
	cCollisionManager::getInstance( )->add( *this );
}
cinder::vec3 const & cAABBCollider::getSize( ) const
{
    return mSize;
}
}
