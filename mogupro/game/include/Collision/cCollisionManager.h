#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/Vector.h>
#include <array>
#include <vector>
#include <set>
#include <tuple>
#include <thread>
#include <chrono>
#include <mutex>
#include <Collision/cRigidBody.h>
#include <cinder/AxisAlignedBox.h>
#include <Collision/cColliderBase.h>
#include <Utility/cUserPointer.hpp>
#include <cinder/Ray.h>
#include <Game/Field/FieldData.h>
namespace Collision
{
class cCollisionManager : public Utility::cSingletonAble<cCollisionManager>
{
public:
    cCollisionManager( );
    ~cCollisionManager( );
    void add( cColliderBase& collider );
    void remove( cColliderBase& collider );
    void add( cRigidBody& rigidBody );
    void remove( cRigidBody& rigidBody );
    void setup( );
    void update( float delta );
    void draw( );
    cinder::vec3 calcNearestPoint( cinder::Ray const& ray, unsigned int layer );
    static constexpr size_t WORLD_X = Game::Field::CHUNK_RANGE_X * Game::Field::CHUNK_SIZE * Game::Field::BLOCK_SIZE;
    static constexpr size_t WORLD_Y = Game::Field::CHUNK_RANGE_Y * Game::Field::CHUNK_SIZE * Game::Field::BLOCK_SIZE;
    static constexpr size_t WORLD_Z = Game::Field::CHUNK_RANGE_Z * Game::Field::CHUNK_SIZE * Game::Field::BLOCK_SIZE;
private:
    bool isRange( int x, int y, int z );
    void fitWorldSpaceMinMax( cinder::ivec3& min, cinder::ivec3& max, cinder::AxisAlignedBox const& aabb ) const;
    std::array<std::array<std::array<std::set<cColliderBase*>, WORLD_Z>, WORLD_Y>, WORLD_X> mWorld;
    std::set<cRigidBody*> mRigidBodys;
private:
    bool mIsUpdate = false;
private:
    bool mDebugDraw = false;
    std::vector<cinder::Ray> mDebugRay;
private:
    struct UpdateThread
    {
        std::thread thread;
		std::mutex mutex;
    };
    std::array<UpdateThread, 4U> mUpdateThreads;
};
}
