#pragma once
#include <cinder/Vector.h>
#include <cinder/Ray.h>
#include <cinder/AxisAlignedBox.h>
#include <vector>
namespace Collision
{
class cColliderBase;
class cRigidBody final
{
public:
    cRigidBody( cColliderBase& collider, cinder::vec3 speed = cinder::vec3( 0.0F ) );
    ~cRigidBody( );
    void addWorld( );
    void removeWorld( );
    cinder::AxisAlignedBox createAABB( ) const;
    void update( float delta );
    void lateUpdate( float delta );
    bool isLanding( ) const;
    bool isGravity( ) const;
	bool isHit( ) const;
    void gravityOn( );
    void gravityOff( );
    cinder::vec3 const& getSpeed( ) const;
    void setSpeed( cinder::vec3 value );
	float getFriction( ) const;
	void setFriction( float value );
    void calc( float minValue, cinder::Ray const& ray, cinder::AxisAlignedBox const& aabb, cColliderBase const* targetCollider );
public:
    cColliderBase& mCollider;
private:
    cinder::vec3 mSpeed;
private:
    bool mIsLanding;
    bool mIsGravity;
	bool mIsHit;
	float mFriction;
private:
    static cinder::vec3 calcWallScratchVector( cinder::vec3 speed, cinder::vec3 normal );
    static cinder::vec3 cardinalAxis( int i );
    static cinder::vec3 getNormal( cinder::vec3 point, cinder::AxisAlignedBox const& box );
};
}
