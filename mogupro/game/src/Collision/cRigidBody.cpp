#include <Collision/cRigidBody.h>
#include <Collision/cCollisionManager.h>
namespace Collision
{
cRigidBody::cRigidBody( cColliderBase& collider, cinder::vec3 speed )
    : mCollider( collider )
    , mSpeed( speed )
    , mIsLanding( false )
{
    cCollisionManager::getInstance( )->add( *this );
}
cRigidBody::~cRigidBody( )
{
    cCollisionManager::getInstance( )->remove( *this );
}
cinder::AxisAlignedBox cRigidBody::createAABB( ) const
{
    auto&& aabb = std::move( mCollider.createAABB( mCollider.mPosition ) );
    auto aPrevPos = mCollider.mPosition - mSpeed;
    aabb.include( std::move( mCollider.createAABB( aPrevPos ) ) );
    return aabb;
}
void cRigidBody::update( )
{
    // “K“–‚Èd—ÍB
    if ( !mIsLanding )
        if ( mSpeed.y > -10.0F )
            mSpeed.y += -0.98F / 60.0F;

    cCollisionManager::getInstance( )->remove( mCollider );
    mCollider.mPosition += mSpeed;
    cCollisionManager::getInstance( )->add( mCollider );
}
}
