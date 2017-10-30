#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/Vector.h>
#include <array>
#include <vector>
#include <set>
#include <tuple>
#include <thread>
#include <chrono>
#include <cinder/Vector.h>
#include <Collision/cRigidBody.h>
#include <cinder/AxisAlignedBox.h>
#include <Collision/cColliderBase.h>
#include <Utility/cRecursionUsableMutex.h>
#include <Utility/cScopedMutex.h>
#include <Collision/cFallBlockSimple.h>
#include <Collision/cStaticBlockSimple.h>
#include <Utility/cUserPointer.hpp>
#include <cinder/Ray.h>
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
    static constexpr size_t WORLD_X = 160;
    static constexpr size_t WORLD_Y = 60;
    static constexpr size_t WORLD_Z = 160;
private:
    bool isRange( int x, int y, int z );
    std::tuple<cinder::ivec3, cinder::ivec3> fitWorldSpaceMinMax( cinder::AxisAlignedBox const& aabb ) const;
    std::mutex mWorldMutex;
    std::array<std::array<std::array<std::set<cColliderBase*>, WORLD_X>, WORLD_Y>, WORLD_Z> mWorld;
    std::mutex mRigidMutex;
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
        Utility::cRecursionUsableMutex mutex;
    };
    std::array<UpdateThread, 4U> mUpdateThreads;
};
}
