#pragma once
#include <Collision/cColliderBase.h>
#include <cinder/Vector.h>
namespace Collision
{
class cAABBCollider final : public cColliderBase
{
public:
    cAABBCollider( cinder::vec3 position, cinder::vec3 size, cinder::vec3 anchor );
    ~cAABBCollider( );
    cinder::AxisAlignedBox createAABB( cinder::vec3 calcedPosition ) const override;
public:
    cinder::vec3 mSize = cinder::vec3( 1.0F );
    cinder::vec3 mAnchor = cinder::vec3( 0.5F );
};
}
