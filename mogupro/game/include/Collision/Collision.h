#pragma once
#include <cinder/Vector.h>
#include <cinder/Ray.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
namespace Collision
{
void hitRayToCube( cinder::Ray const & ray, unsigned int layer, cColliderBase * bCollider, float & calcMin, cinder::Ray & calcRay, cinder::AxisAlignedBox & calcBoundingBox, cColliderBase ** targetCollider );
void hitRayToCube( cinder::Ray const& ray, cAABBCollider * bCollider, float& calcMin, cinder::Ray& calcRay, cinder::AxisAlignedBox& calcBoundingBox, cColliderBase** targetCollider );
void hitCubeToCube( cColliderBase * aCollider, cRigidBody * aRigidBody, cColliderBase * bCollider, float& min, cinder::Ray& ray, cinder::AxisAlignedBox& boundingBox, cColliderBase** targetCollider );
void hitCubeToCube( cAABBCollider * aAABB, cRigidBody * aRigidBody, cAABBCollider * bAABB, float& min, cinder::Ray& ray, cinder::AxisAlignedBox& boundingBox, cColliderBase** targetCollider );
}
