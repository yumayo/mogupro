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

    AxisAlignedBox boundingBox( -aSize * 0.5F + bSize * 0.5F, aSize * ( 1.0F - 0.5F ) + bSize * ( 1.0F - 0.5F ) );
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
void hitCubeToCube( cColliderBase * aCollider, cRigidBody * aRigidBody, cColliderBase * bCollider )
{
    if ( aCollider == bCollider ) return;
    if ( aCollider->getType( ) == cColliderBase::Type::AABB &&
         bCollider->getType( ) == cColliderBase::Type::AABB )
    {
        hitCubeToCube( dynamic_cast<cAABBCollider*>( aCollider ), aRigidBody,
                              dynamic_cast<cAABBCollider*>( bCollider ) );
    }
}
void hitCubeToCube( cAABBCollider * aAABB, cRigidBody * aRigidBody, cAABBCollider * bAABB )
{
    auto aBox = aAABB->createAABB( aAABB->getPosition( ) );
    auto bBox = bAABB->createAABB( bAABB->getPosition( ) );
    if ( aBox.intersects( bBox ) )
    {
        auto aPrevPos = aAABB->getPosition( ) - aRigidBody->getSpeed( );
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

        Ray ray( aPrevPos, direction );

        AxisAlignedBox boundingBox( -aAABB->getSize( ) * aAABB->getAnchor( ) + -bAABB->getSize( ) * bAABB->getAnchor( ),
                                    aAABB->getSize( ) * ( 1.0F - aAABB->getAnchor( ) ) + bAABB->getSize( ) * ( 1.0F - bAABB->getAnchor( ) ) );
        boundingBox.transform( translate( mat4( ), bAABB->getPosition( ) ) );

        float min = 0.0F, max = 0.0F;
        if ( boundingBox.intersect( ray, &min, &max ) != 0 )
        {
            if ( min >= 0.0F && min <= 1.0F )
            {
                aRigidBody->calc( min, ray, boundingBox, bAABB );
            }
        }
    }
}
}
