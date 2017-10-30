#include <Collision/cCollisionManager.h>
#include <cinder/gl/gl.h>
#include <Collision/Collision.h>
#include <Utility/cString.h>
#include <Utility/cInput.h>
using namespace cinder;
namespace Collision
{
cCollisionManager::cCollisionManager( )
{
}
cCollisionManager::~cCollisionManager( )
{
}
void cCollisionManager::add( cColliderBase& collider )
{
    std::vector<std::set<cColliderBase*>*> leafs;
    auto&& aabb = std::move( collider.createAABB( collider.getPosition( ) ) );
    auto&& minMax = std::move( fitWorldSpaceMinMax( aabb ) );
    ivec3 min = std::get<0>( minMax );
    ivec3 max = std::get<1>( minMax );
    for ( int x = min.x; x <= max.x; ++x )
    {
        for ( int y = min.y; y <= max.y; ++y )
        {
            for ( int z = min.z; z <= max.z; ++z )
            {
                leafs.emplace_back( &mWorld[x][y][z] );
            }
        }
    }
    {
        std::lock_guard<std::mutex> mutex( mWorldMutex );
        for ( auto& leaf : leafs )
        {
            leaf->insert( &collider );
        }
    }
    collider.setLeafs( std::move( leafs ) );
}
void cCollisionManager::remove( cColliderBase& collider )
{
    for ( auto& leaf : collider.getLeafs( ) )
    {
        leaf->erase( &collider );
    }
}
void cCollisionManager::add( cRigidBody& rigidBody )
{
    std::lock_guard<std::mutex> mutex( mRigidMutex );
    mRigidBodys.insert( &rigidBody );
}
void cCollisionManager::remove( cRigidBody & rigidBody )
{
    std::lock_guard<std::mutex> mutex( mRigidMutex );
    mRigidBodys.erase( &rigidBody );
}
void cCollisionManager::setup( )
{
}
void cCollisionManager::update( float delta )
{
    if ( ENV->pushKey( cinder::app::KeyEvent::KEY_u ) )
    {
        mIsUpdate = !mIsUpdate;
    }
    if ( !ENV->pushKey( cinder::app::KeyEvent::KEY_i ) )
    {
        if ( !mIsUpdate ) return;
    }
    // TODO: Ç±Ç±Ç©ÇÁêÊÇThreadâªÇ∑ÇÈÅB

    for ( auto& rigidBody : mRigidBodys )
    {
        rigidBody->update( delta );
    }
    for ( auto& rigidBody : mRigidBodys )
    {
    recalc:
        auto&& aabb = std::move( rigidBody->createAABB( ) );
        auto&& minMax = std::move( fitWorldSpaceMinMax( aabb ) );
        ivec3 min = std::get<0>( minMax );
        ivec3 max = std::get<1>( minMax );
        float length = std::numeric_limits<float>::max( );
        cinder::Ray ray;
        cinder::AxisAlignedBox boundingBox;
        cColliderBase const* targetCollider = nullptr;
        for ( int x = min.x; x <= max.x; ++x )
        {
            for ( int y = min.y; y <= max.y; ++y )
            {
                for ( int z = min.z; z <= max.z; ++z )
                {
                    auto const& rigidCollider = rigidBody->mCollider;
                    auto const& colliders = mWorld[x][y][z];
                    for ( auto const& collider : colliders )
                    {
                        hitCubeToCube( &rigidCollider, rigidBody, collider, length, ray, boundingBox, &targetCollider );
                    }
                }
            }
        }
        if ( targetCollider != nullptr )
        {
            if ( cinder::length( ray.getDirection( ) ) < 0.01F )
            {
                rigidBody->mCollider.setPosition( rigidBody->mCollider.getPosition( ) - rigidBody->getSpeed( ) );
                continue;
            }
            rigidBody->calc( length, ray, boundingBox, targetCollider );
            goto recalc;
        }
    }
    for ( auto& rigidBody : mRigidBodys )
    {
        rigidBody->lateUpdate( delta );
    }
}
void cCollisionManager::draw( )
{
}
cinder::vec3 cCollisionManager::calcNearestPoint( cinder::Ray const & ray, unsigned int layer )
{
    cinder::AxisAlignedBox aabb;
    aabb.include( ray.getOrigin( ) );
    aabb.include( ray.getOrigin( ) + ray.getDirection( ) );
    auto&& minMax = std::move( fitWorldSpaceMinMax( aabb ) );
    ivec3 min = std::get<0>( minMax );
    ivec3 max = std::get<1>( minMax );

    float calcMin = std::numeric_limits<float>::max( );
    cinder::Ray calcRay;
    cinder::AxisAlignedBox calcBoundingBox;
    cColliderBase const* targetCollider = nullptr;

    for ( int x = min.x; x <= max.x; ++x )
    {
        for ( int y = min.y; y <= max.y; ++y )
        {
            for ( int z = min.z; z <= max.z; ++z )
            {
                auto const& colliders = mWorld[x][y][z];
                for ( auto& collider : colliders )
                {
                    hitRayToCube( ray, layer, collider, calcMin, calcRay, calcBoundingBox, &targetCollider );
                }
            }
        }
    }

    if ( targetCollider != nullptr )
    {
        return calcRay.calcPosition( calcMin );
    }
    return ray.calcPosition( 1.0F );
}
bool cCollisionManager::isRange( int x, int y, int z )
{
    return 0 <= x && 0 <= y && 0 <= z &&
        x < WORLD_X && y < WORLD_Y && z < WORLD_Z;
}
std::tuple<cinder::ivec3, cinder::ivec3> cCollisionManager::fitWorldSpaceMinMax( cinder::AxisAlignedBox const& aabb ) const
{
    ivec3 min = floor( aabb.getMin( ) );
    vec3 ceilMax = floor( aabb.getMax( ) );
    vec3 defaultMax = aabb.getMax( );

    // maxÇÃè¨êîì_à»â∫Ç™0ÇÃèÍçá8Ç¬ï™ìoò^Ç≥ÇÍÇƒÇµÇ‹Ç§ÇÃÇ≈ÇªÇÍÇñhÇÆÅB
    ivec3 max = ceilMax == defaultMax ? ceilMax - 1.0F : ceilMax;

    min = clamp( min, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
    max = clamp( max, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );

    return std::make_tuple( min, max );
}
}
