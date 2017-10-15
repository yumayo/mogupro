#include <Collision/Collision.h>
#include <cinder/AxisAlignedBox.h>
#include <cinder/Ray.h>
using namespace cinder;
namespace Collision
{
bool hitCubeToCube( cinder::vec3 * const result, cinder::vec3 aPos, cinder::vec3 aSpeed, cinder::vec3 aSize, cinder::vec3 bPos, cinder::vec3 bSize )
{
    auto aPrevPos = aPos - aSpeed;
    auto direction = aPos - aPrevPos;
    Ray ray( aPrevPos, direction );

    AxisAlignedBox boundingBox( -aSize * 0.5F + bSize * 0.5F, aSize * 0.5F + bSize * 0.5F );
    boundingBox.transform( translate( mat4( ), bPos ) );

    float min = 0.0F, max = 0.0F;
    int isHit = boundingBox.intersect( ray, &min, &max );
    if ( isHit == 0 ) return false;
    else
    {
        *result = ray.calcPosition( min );
        return true;
    }
}
bool hitCubeToCube( cColliderBase * aCollider, cRigidBody * aRigidBody, cColliderBase * bCollider )
{
    if ( aCollider == bCollider ) return false;
    if ( aCollider->mType == cColliderBase::Type::AABB &&
         bCollider->mType == cColliderBase::Type::AABB )
    {
        return hitCubeToCube( dynamic_cast<cAABBCollider*>( aCollider ), aRigidBody,
                              dynamic_cast<cAABBCollider*>( bCollider ) );
    }
    return false;
}
bool hitCubeToCube( cAABBCollider * aAABB, cRigidBody * aRigidBody, cAABBCollider * bAABB )
{
    auto aPrevPos = aAABB->mPosition - aRigidBody->mSpeed;
    auto direction = aAABB->mPosition - aPrevPos;
    Ray ray( aPrevPos, direction );

    AxisAlignedBox boundingBox( -aAABB->mSize * aAABB->mAnchor + -bAABB->mSize * bAABB->mAnchor,
                                aAABB->mSize * aAABB->mAnchor + bAABB->mSize * bAABB->mAnchor );
    boundingBox.transform( translate( mat4( ), bAABB->mPosition ) );

    float min = 0.0F, max = 0.0F;
    int isHit = boundingBox.intersect( ray, &min, &max );
    if ( isHit != 0 )
    {
        aAABB->mPosition = ray.calcPosition( min );
        aRigidBody->mSpeed *= 0.1F;
    }
    return isHit;
}
}
