#pragma once
#include <cinder/Vector.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
#include <Utility/cUserPointer.hpp>
namespace Collision
{
class cFallBlockSimple
{
public:
    cFallBlockSimple( cinder::vec3 pos );
    void draw( );
private:
    Utility::hardptr<Collision::cAABBCollider> mCollider;
    Utility::hardptr<Collision::cRigidBody> mRigidbody;
};
}
