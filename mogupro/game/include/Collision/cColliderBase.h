#pragma once
#include <cinder/Vector.h>
#include <cinder/AxisAlignedBox.h>
#include <vector>
#include <set>
namespace Collision
{
class cRigidBody;
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
    Type getType( );
    cinder::vec3 const& getPosition( );
    void update( cRigidBody* rigidbody );
    void setLeafs( std::vector<std::set<cColliderBase*>*>&& leafs );
    std::vector<std::set<cColliderBase*>*> const& getLeafs( );
    void calc( cinder::vec3 position );
protected:
    Type mType;
    cinder::vec3 mPosition;
    std::vector<std::set<cColliderBase*>*> mLeafs;
};
}
