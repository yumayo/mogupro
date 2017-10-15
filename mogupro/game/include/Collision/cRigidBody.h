#pragma once
#include <cinder/Vector.h>
#include <Collision/cColliderBase.h>
#include <cinder/AxisAlignedBox.h>
namespace Collision
{
class cRigidBody final
{
public:
    cRigidBody( cColliderBase& collider, cinder::vec3 speed );
    ~cRigidBody( );
    cinder::AxisAlignedBox createAABB( ) const;
    void update( );
public:
    cColliderBase& mCollider;
    bool mIsLanding;
    cinder::vec3 mSpeed;
};
}
