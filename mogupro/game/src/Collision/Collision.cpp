#include <Collision/Collision.h>
#include <cinder/AxisAlignedBox.h>
#include <cinder/Ray.h>
using namespace cinder;
namespace Collision
{
void hitRayToCube( cinder::Ray const & ray, unsigned int layer, cColliderBase const* const bCollider, float & calcMin, cinder::Ray & calcRay, cinder::AxisAlignedBox & calcBoundingBox, cColliderBase const** targetCollider )
{
    if ( ( bCollider->getLayer( ) & layer ) == 0U )
        return;

    if ( bCollider->getType( ) == cColliderBase::Type::AABB )
    {
        hitRayToCube( ray,
                      dynamic_cast<cAABBCollider const*>( bCollider ),
                      calcMin, calcRay, calcBoundingBox, targetCollider );
    }
}
void hitRayToCube( cinder::Ray const & ray, cAABBCollider const* const bCollider, float & calcMin, cinder::Ray & calcRay, cinder::AxisAlignedBox & calcBoundingBox, cColliderBase const** targetCollider )
{
    AxisAlignedBox boundingBox( cinder::vec3( 0.0F ) + -bCollider->getSize( ) / 2.0F,
                                cinder::vec3( 0.0F ) + bCollider->getSize( ) / 2.0F );
    boundingBox.transform( translate( mat4( ), bCollider->getPosition( ) ) );

    float min = 0.0F, max = 0.0F;
    if ( boundingBox.intersect( ray, &min, &max ) != 0 )
    {
        if ( min >= 0.0F && min <= 1.0F )
        {
            if ( min < calcMin )
            {
                calcMin = min;
                calcRay = ray;
                calcBoundingBox = boundingBox;
                *targetCollider = bCollider;
            }
        }
    }
}
void hitCubeToCube( cColliderBase const* const aCollider, cRigidBody const* const aRigidBody, cColliderBase const* const bCollider, float& min, cinder::Ray& ray, cinder::AxisAlignedBox& boundingBox, cColliderBase const** targetCollider )
{
    if ( ( bCollider->getLayer( ) & aCollider->getLayer( ) ) == 0U )
        return;

    if ( aCollider == bCollider ) return;
    if ( aCollider->getType( ) == cColliderBase::Type::AABB &&
         bCollider->getType( ) == cColliderBase::Type::AABB )
    {
        hitCubeToCube( dynamic_cast<cAABBCollider const*>( aCollider ), aRigidBody,
                       dynamic_cast<cAABBCollider const*>( bCollider ),
                       min, ray, boundingBox, targetCollider );
    }
}
void hitCubeToCube( cAABBCollider const* const aAABB, cRigidBody const* const aRigidBody, cAABBCollider const* const bAABB, float& min, cinder::Ray& ray, cinder::AxisAlignedBox& boundingBox, cColliderBase const** targetCollider )
{
    auto aPrevPos = aAABB->getPosition( ) - aRigidBody->getSpeedCalcedDelta( );
    auto direction = aAABB->getPosition( ) - aPrevPos;
    //@ Q^PPPP_Q
    //@(›@ LƒÖM@ ›)
    //@/@ ^PPP_@ƒ©
    //`/@^/‚u| ‚m‚u|_ ƒ©
    //b /‚u _R| ^Rƒ© b
    //bbb(œ). (œ)| bb
    //bbl(PP)| bb
    //b(Ú‚u„¤“ñ“ñ_Ú|/Ø b
    //b ___Q(o_Qƒm“ñDb
    //b@/‚V@@@@R @b
    //b / b@^@@ |ƒ© b
    //l(Ú| L_QQQQ|‚m)É
    //@_R|-/‚u)É|-|ƒm^
    //@@P(/PP R)P

    Ray calcRay( aPrevPos, direction );

    AxisAlignedBox calcBoundingBox( -aAABB->getSize( ) / 2.0F + -bAABB->getSize( ) / 2.0F,
                                    aAABB->getSize( ) / 2.0F + bAABB->getSize( ) / 2.0F );
    calcBoundingBox.transform( translate( mat4( ), bAABB->getPosition( ) ) );

    float calcMin = std::numeric_limits<float>::max( ), calcMax = std::numeric_limits<float>::max( );
    if ( calcBoundingBox.intersect( calcRay, &calcMin, &calcMax ) != 0 )
    {
        if ( calcMin > 0.0F - 0.005F && calcMin < 1.0F + 0.005F )
        {
            if ( calcMin < min )
            {
                min = calcMin;
                ray = calcRay;
                boundingBox = calcBoundingBox;
                *targetCollider = bAABB;
            }
        }
    }
}
}
