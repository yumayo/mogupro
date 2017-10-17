#pragma once
#include <cinder/Vector.h>
#include <cinder/AxisAlignedBox.h>
namespace Collision
{
class cColliderBase;
class cRigidBody final
{
public:
    cRigidBody( cColliderBase& collider, cinder::vec3 speed = cinder::vec3( 0.0F ) );
    ~cRigidBody( );
    cinder::AxisAlignedBox createAABB( ) const;
    void update( );
    bool isLanding( );
    cinder::vec3 const& getSpeed( );
    void setSpeed( cinder::vec3 speed );
public:
    cColliderBase& mCollider;
private:
    bool mIsLanding;
    cinder::vec3 mSpeed;
};
}
