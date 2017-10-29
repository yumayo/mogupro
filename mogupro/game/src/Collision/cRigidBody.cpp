#include <Collision/cRigidBody.h>
#include <Collision/cColliderBase.h>
#include <Collision/cCollisionManager.h>
#include <limits>
#include <cinder/app/App.h>
namespace Collision
{
cRigidBody::cRigidBody( cColliderBase& collider, cinder::vec3 speed )
    : mCollider( collider )
    , mSpeed( speed )
    , mMinValue( std::numeric_limits<float>::max( ) )
    , mIsGravity( true )
    , mIsLanding( false )
{
}
cRigidBody::~cRigidBody( )
{
}
void cRigidBody::addWorld( )
{
    cCollisionManager::getInstance( )->add( *this );
}
void cRigidBody::removeWorld( )
{
    cCollisionManager::getInstance( )->remove( *this );
}
cinder::AxisAlignedBox cRigidBody::createAABB( ) const
{
    auto&& aabb = std::move( mCollider.createAABB( mCollider.getPosition( ) ) );
    auto aPrevPos = mCollider.getPosition( ) - mSpeed;
    aabb.include( std::move( mCollider.createAABB( aPrevPos ) ) );
    return aabb;
}

void cRigidBody::update( float delta )
{
    mMinValue = std::numeric_limits<float>::max( );
    mIsLanding = false;

    if ( mIsGravity )
    {
        if ( mSpeed.y > -10.0F )
            mSpeed.y += -0.98F * delta;
    }

    mCollider.update( this );
}
void cRigidBody::lateUpdate( float delta )
{
    if ( mMinValue != std::numeric_limits<float>::max( ) )
    {
        mSpeed *= ( 1.0F - 0.08 ) * delta;
    }
}
bool cRigidBody::isLanding( )
{
    return mIsLanding;
}
bool cRigidBody::isGravity( )
{
    return mIsGravity;
}
void cRigidBody::gravityOn( )
{
    mIsGravity = true;
}
void cRigidBody::gravityOff( )
{
    mIsGravity = false;
}
cinder::vec3 const & cRigidBody::getSpeed( )
{
    return mSpeed;
}
void cRigidBody::setSpeed( cinder::vec3 value )
{
    mSpeed = value;
}
cinder::vec3 cRigidBody::cardinalAxis( int i )
{
    switch ( i )
    {
    case 0:
        return cinder::vec3( 1, 0, 0 );
    case 1:
        return cinder::vec3( 0, 1, 0 );
    case 2:
        return cinder::vec3( 0, 0, 1 );
    default:
        throw std::runtime_error( "‚â‚Î‚¢‚æ‚â‚Î‚¢‚æ" );
    }
}
// https://www.gamedev.net/forums/topic/551816-finding-the-aabb-surface-normal-from-an-intersection-point-on-aabb/
cinder::vec3 cRigidBody::getNormal( cinder::vec3 point, cinder::AxisAlignedBox const& box )
{
    cinder::vec3 normal = cinder::vec3( 0 );
    float min_distance = std::numeric_limits<float>::max( );
    point -= box.getCenter( );

    for ( int i = 0; i < 3; ++i ) {
        float distance = abs( box.getExtents( )[i] - abs( point[i] ) );
        if ( distance < min_distance ) {
            min_distance = distance;
            normal = glm::sign( point[i] ) * cardinalAxis( i );
        }
    }
    return normal;
}
cinder::vec3 cRigidBody::calcWallScratchVector( cinder::vec3 spd, cinder::vec3 nor )
{
    float t = -( nor.x * spd.x + nor.y * spd.y + nor.z * spd.z ) / ( nor.x * nor.x + nor.y * nor.y + nor.z * nor.z );
    return cinder::vec3( spd.x + t * nor.x, spd.y + t * nor.y, spd.z + t * nor.z );
}
void cRigidBody::calc( float minValue, cinder::Ray const& ray, cinder::AxisAlignedBox const& aabb, cColliderBase* targetCollider )
{
    auto intersectPoint = ray.calcPosition( minValue - 0.005F );
    auto normal = getNormal( intersectPoint, aabb );

    if ( normal.y == 1.0F )
    {
        mIsLanding = true;
    }

    mSpeed = calcWallScratchVector( mSpeed, normal ) * 0.9F;

    auto position = intersectPoint + mSpeed;

    mCollider.setPosition( position );
}
}
