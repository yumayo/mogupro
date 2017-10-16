#pragma once
#include <cinder/Vector.h>
#include <cinder/AxisAlignedBox.h>
namespace Collision
{
class cColliderBase
{
public:
    enum class Type
    {
        AABB,
    };
public:
    cColliderBase( Type colliderType, cinder::vec3 position );
    virtual ~cColliderBase( ) { }
    virtual cinder::AxisAlignedBox createAABB( cinder::vec3 calcedPosition ) const = 0;
public:
    cinder::vec3 mPosition = cinder::vec3( 0.0F );
    Type mType;
};
}
