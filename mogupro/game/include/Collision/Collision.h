#pragma once
#include <cinder/Vector.h>
#include <Collision/cAABBCollider.h>
#include <Collision/cRigidBody.h>
namespace Collision
{
// 動的なCubeと静的なCubeの当たり判定を行います。
// 返り値: 当たり判定を起こした場合 true
// result: 当たり判定を起こした時に動いているCubeの位置が入ります。
// aPos: 動いているCubeの現在位置
// aSpeed: 動いているCube移動量
// aSize: 動いているCubeの大きさ
// bPos: 止まっているCubeの位置
// bSize: 止まっているCubeの大きさ
bool hitCubeToCube( cinder::vec3* const result, cinder::vec3 aPos, cinder::vec3 aSpeed, cinder::vec3 aSize,
                    cinder::vec3 bPos, cinder::vec3 bSize );
void hitCubeToCube( cColliderBase * aCollider, cRigidBody * aRigidBody, cColliderBase * bCollider, float& min, cinder::Ray& ray, cinder::AxisAlignedBox& boundingBox, cColliderBase** targetCollider );
void hitCubeToCube( cAABBCollider * aAABB, cRigidBody * aRigidBody, cAABBCollider * bAABB, float& min, cinder::Ray& ray, cinder::AxisAlignedBox& boundingBox, cColliderBase** targetCollider );
}
