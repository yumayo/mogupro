#pragma once
#include <Collision/cColliderBase.h>
#include <cinder/Vector.h>
#include <mutex>
namespace Collision
{
class cAABBCollider final : public cColliderBase
{
public:
	cAABBCollider( );
    cAABBCollider( cinder::vec3 position, cinder::vec3 size = cinder::vec3( 1.0F ) );
    ~cAABBCollider( );
    void addWorld( );
    void removeWorld( );
    cinder::AxisAlignedBox createAABB( cinder::vec3 calcedPosition ) const override;
	void setSize( cinder::vec3 const& value );
    cinder::vec3 const& getSize( ) const;
private:
    cinder::vec3 mSize;
};
}
