#include <Collision/cCollisionManager.h>
#include <cinder/gl/gl.h>
#include <Collision/Collision.h>
#include <Utility/cString.h>
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
    min = clamp( min, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
    max = clamp( max, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
    for ( int x = min.x; x <= max.x; ++x )
    {
        for ( int y = min.y; y <= max.y; ++y )
        {
            for ( int z = min.z; z <= max.z; ++z )
            {
                leafs.emplace_back( &mWorld[x][y][z] );
                leafs.back( )->insert( &collider );
            }
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
    mRigidBodys.insert( &rigidBody );
}
void cCollisionManager::remove( cRigidBody & rigidBody )
{
    mRigidBodys.erase( &rigidBody );
}
void cCollisionManager::setup( )
{
    mStaticBlock = std::make_shared<cStaticBlockSimple>( cinder::vec3( 10, 2, 10 ), cinder::vec3( 22, 4, 22 ) );
    mFallBlocks.reserve( 32 );
    for ( int x = 0; x < 2; ++x )
    {
        for ( int y = 0; y < 2; ++y )
        {
            for ( int z = 0; z < 2; ++z )
            {
                mFallBlocks.emplace_back( vec3( x * 2, 10 + y * 4, z * 2 ) );
            }
        }
    }
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
    for ( auto& b : mFallBlocks ) b.draw( );
    mStaticBlock->draw( );
}
bool cCollisionManager::isRange( int x, int y, int z )
{
    return 0 <= x && 0 <= y && 0 <= z &&
        x < WORLD_X && y < WORLD_Y && z < WORLD_Z;
}
void cCollisionManager::broadphase( )
{

}
void cCollisionManager::narophase( )
{

}
std::tuple<cinder::ivec3, cinder::ivec3> cCollisionManager::fitWorldSpaceMinMax( cinder::AxisAlignedBox const& aabb )
{
    ivec3 min = floor( aabb.getMin( ) );
    vec3 ceilMax = ceil( aabb.getMax( ) );
    vec3 defaultMax = aabb.getMax( );

    // maxÇÃè¨êîì_à»â∫Ç™0ÇÃèÍçá8Ç¬ï™ìoò^Ç≥ÇÍÇƒÇµÇ‹Ç§ÇÃÇ≈ÇªÇÍÇñhÇÆÅB
    ivec3 max = ceilMax == defaultMax ? ceilMax - 1.0F : ceilMax;

    return std::make_tuple( min, max );
}
}
