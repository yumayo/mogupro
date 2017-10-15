#pragma once
#include <cinder/Vector.h>
#include <Collision/cColliderBase.h>
#include <Utility/cUserPointer.hpp>
namespace Collision
{
class cCollisionObject
{
public:
    enum class CollisionType : unsigned int
    {
        AABB,
    };
    cCollisionObject( ) { }
    virtual ~cCollisionObject( ) { }
public:
    Utility::hardptr<cColliderBase> mCollider;
};
}
