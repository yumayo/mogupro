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
    ivec3 min, max;
    fitWorldSpaceMinMax( min, max, aabb );
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
    mRigidBodys.insert( &rigidBody );
}
void cCollisionManager::remove( cRigidBody & rigidBody )
{
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
    // TODO: ‚±‚±‚©‚çæ‚ðThread‰»‚·‚éB

    for ( auto& rigidBody : mRigidBodys )
    {
        rigidBody->update( delta );
    }
    for ( auto& rigidBody : mRigidBodys )
    {
        int i = 0;
        for ( ; i < 10; ++i )
        {
            auto&& aabb = std::move( rigidBody->createAABB( ) );
            ivec3 min, max;
            fitWorldSpaceMinMax( min, max, aabb );
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
            if ( targetCollider == nullptr ) break;
            rigidBody->calc( length, ray, boundingBox, targetCollider );
        }
        // 10‰ñˆÈãŒJ‚è•Ô‚µ‚½ê‡‚Íundo‚µ‚Ä•Ô‚µ‚Ü‚·B
        if ( i == 10 )
        {
            rigidBody->mCollider.setPosition( rigidBody->mCollider.getPosition( ) - rigidBody->getSpeed( ) );
        }
    }
    for ( auto& rigidBody : mRigidBodys )
    {
        rigidBody->lateUpdate( delta );
    }
}
void cCollisionManager::draw( )
{
    if ( ENV->pressKey( ci::app::KeyEvent::KEY_F3 ) && ENV->pushKey( ci::app::KeyEvent::KEY_b ) )
    {
        mDebugDraw = !mDebugDraw;
    }

    mDebugRay.clear( );
    if ( !mDebugDraw ) return;

    cinder::gl::ScopedColor white( Color( 1, 1, 1 ) );
    for ( auto const& rigidBody : mRigidBodys )
    {
        switch ( rigidBody->mCollider.getType( ) )
        {
        case cColliderBase::Type::AABB:
        {
            cAABBCollider const* aabbCo = dynamic_cast<cAABBCollider const*>( &rigidBody->mCollider );
            cinder::gl::drawStrokedCube( rigidBody->mCollider.getPosition( ), aabbCo->getSize( ) );
        }
        break;
        default:
            break;
        }
    }
    cinder::gl::ScopedColor red( Color( 1, 0, 0 ) );
    for ( auto const& ray : mDebugRay )
    {
        cinder::gl::drawLine( ray.getOrigin( ), ray.getOrigin( ) + ray.getDirection( ) );
    }
}
cinder::vec3 cCollisionManager::calcNearestPoint( cinder::Ray const & ray, unsigned int layer )
{
    mDebugRay.emplace_back( ray );
    auto rayMin = ray.getOrigin( );
    auto rayMax = ray.getOrigin( ) + ray.getDirection( );
    cinder::AxisAlignedBox aabb( rayMin, rayMax );
    ivec3 min, max;
    fitWorldSpaceMinMax( min, max, aabb );
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
void cCollisionManager::fitWorldSpaceMinMax( cinder::ivec3& min, cinder::ivec3& max, cinder::AxisAlignedBox const& aabb ) const
{
    min = floor( aabb.getMin( ) );
    vec3 floorMax = floor( aabb.getMax( ) );
    vec3 defaultMax = aabb.getMax( );

    // max‚Ì¬”“_ˆÈ‰º‚ª0‚Ìê‡8‚Â•ª“o˜^‚³‚ê‚Ä‚µ‚Ü‚¤‚Ì‚Å‚»‚ê‚ð–h‚®B
    max = floorMax == defaultMax ? floorMax - 1.0F : floorMax;

    min = clamp( min, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
    max = clamp( max, ivec3( 0 ), ivec3( WORLD_X - 1, WORLD_Y - 1, WORLD_Z - 1 ) );
}
}
