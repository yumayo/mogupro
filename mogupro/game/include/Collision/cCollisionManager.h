#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/Vector.h>
#include <array>
#include <vector>
#include <set>
#include <cinder/Vector.h>
#include <Collision/cRigidBody.h>
#include <cinder/AxisAlignedBox.h>
#include <tuple>
#include <Collision/cColliderBase.h>
#include <thread>
#include <Utility/cRecursionUsableMutex.h>
#include <Utility/cScopedMutex.h>
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
    void update( );
    void draw( );
    static constexpr size_t WORLD_X = 160;
    static constexpr size_t WORLD_Y = 60;
    static constexpr size_t WORLD_Z = 160;
private:
    bool isRange( int x, int y, int z );
    std::tuple<cinder::ivec3, cinder::ivec3> fitWorldSpaceMinMax( cinder::AxisAlignedBox const& aabb );
    std::array<std::array<std::array<std::set<cColliderBase*>, WORLD_X>, WORLD_Y>, WORLD_Z> mWorld;
    std::set<cRigidBody*> mRigidBodys;
};
}