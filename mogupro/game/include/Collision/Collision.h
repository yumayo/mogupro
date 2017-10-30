#pragma once
#include <cinder/Vector.h>
#include <cinder/Ray.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
namespace Collision
{
void hitRayToCube( cinder::Ray const & ray, unsigned int layer, cColliderBase const* const bCollider, float & calcMin, cinder::Ray & calcRay, cinder::AxisAlignedBox & calcBoundingBox, cColliderBase const** targetCollider );
void hitRayToCube( cinder::Ray const& ray, cAABBCollider const* const bCollider, float& calcMin, cinder::Ray& calcRay, cinder::AxisAlignedBox& calcBoundingBox, cColliderBase const** targetCollider );
void hitCubeToCube( cColliderBase const* const aCollider, cRigidBody const* const aRigidBody, cColliderBase const* const bCollider, float& min, cinder::Ray& ray, cinder::AxisAlignedBox& boundingBox, cColliderBase const** targetCollider );
void hitCubeToCube( cAABBCollider const* const aAABB, cRigidBody const* const aRigidBody, cAABBCollider const* const bAABB, float& min, cinder::Ray& ray, cinder::AxisAlignedBox& boundingBox, cColliderBase const** targetCollider );
}
