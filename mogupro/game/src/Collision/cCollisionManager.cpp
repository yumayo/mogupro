#include <Collision/cCollisionManager.h>
#include <cinder/gl/gl.h>
#include <Collision/Collision.h>
#include <Utility/cString.h>
#include <chrono>
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
    auto&& aabb = std::move( collider.createAABB( collider.mPosition ) );
    auto&& minMax = std::move( fitWorldSpaceMinMax( aabb ) );
    ivec3 min = std::get<0>( minMax );
    ivec3 max = std::get<1>( minMax );
    min = clamp( min, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
    max = clamp( max, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
    for ( int x = min.x; x <= max.x; ++x )
    {
        for ( int y = min.y; y <= max.y; ++y )
        {
            for ( int z = min.z; z <= max.z; ++z )
            {
                mWorld[x][y][z].insert( &collider );
            }
        }
    }
}
void cCollisionManager::remove( cColliderBase& collider )
{
    auto&& aabb = std::move( collider.createAABB( collider.mPosition ) );
    auto&& minMax = std::move( fitWorldSpaceMinMax( aabb ) );
    ivec3 min = std::get<0>( minMax );
    ivec3 max = std::get<1>( minMax );
    min = clamp( min, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
    max = clamp( max, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
    for ( int x = min.x; x <= max.x; ++x )
    {
        for ( int y = min.y; y <= max.y; ++y )
        {
            for ( int z = min.z; z <= max.z; ++z )
            {
                mWorld[x][y][z].erase( &collider );
            }
        }
    }
}
void cCollisionManager::add( cRigidBody& rigidBody )
{
    mRigidBodys.insert( &rigidBody );
}
void cCollisionManager::remove( cRigidBody & rigidBody )
{
    mRigidBodys.erase( &rigidBody );
}
void cCollisionManager::update( )
{
    for ( auto& rigidBody : mRigidBodys )
    {
        rigidBody->update( );
        auto&& aabb = std::move( rigidBody->createAABB( ) );
        auto&& minMax = std::move( fitWorldSpaceMinMax( aabb ) );
        ivec3 min = std::get<0>( minMax );
        ivec3 max = std::get<1>( minMax );
        min = clamp( min, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
        max = clamp( max, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
        for ( int x = min.x; x <= max.x; ++x )
        {
            for ( int y = min.y; y <= max.y; ++y )
            {
                for ( int z = min.z; z <= max.z; ++z )
                {
                    auto& rigidCollider = rigidBody->mCollider;
                    auto& colliders = mWorld[x][y][z];
                    for ( auto& collider : colliders )
                    {
                        hitCubeToCube( &rigidCollider, rigidBody, collider );
                    }
                }
            }
        }
    }
}
void cCollisionManager::draw( )
{
}
bool cCollisionManager::isRange( int x, int y, int z )
{
    return 0 <= x && 0 <= y && 0 <= z &&
        x < WORLD_X && y < WORLD_Y && z < WORLD_Z;
}
std::tuple<cinder::ivec3, cinder::ivec3> cCollisionManager::fitWorldSpaceMinMax( cinder::AxisAlignedBox const& aabb )
{
    ivec3 min = floor( aabb.getMin( ) );
    vec3 ceilMax = ceil( aabb.getMax( ) );
    vec3 defaultMax = aabb.getMax( );

    // max�̏����_�ȉ���0�̏ꍇ8���o�^����Ă��܂��̂ł����h���B
    ivec3 max = ceilMax == defaultMax ? ceilMax - 1.0F : ceilMax;

    return std::make_tuple( min, max );
}
}