#pragma once
#include <cinder/Vector.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
namespace Collision
{
class cFallBlockSimple
{
public:
    cFallBlockSimple( cinder::vec3 pos );
    void draw( );
private:
    Collision::cAABBCollider mCollider;
    Collision::cRigidBody mRigidbody;
};
}
