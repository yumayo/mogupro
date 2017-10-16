#pragma once
#include <Collision/cColliderBase.h>
#include <cinder/Vector.h>
namespace Collision
{
class cAABBCollider final : public cColliderBase
{
public:
    cAABBCollider( cinder::vec3 position, cinder::vec3 size = cinder::vec3( 1.0F ), cinder::vec3 anchor = cinder::vec3( 0.5F ) );
    ~cAABBCollider( );
    cinder::AxisAlignedBox createAABB( cinder::vec3 calcedPosition ) const override;
    cinder::vec3 const& getSize( );
    cinder::vec3 const& getAnchor( );
private:
    cinder::vec3 mSize;
    cinder::vec3 mAnchor;
};
}
