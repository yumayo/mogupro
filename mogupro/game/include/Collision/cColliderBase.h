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
    enum class Type : unsigned char
    {
        AABB,
    };
public:
    cColliderBase( Type colliderType, cinder::vec3 position );
    virtual ~cColliderBase( ) { }
    virtual cinder::AxisAlignedBox createAABB( cinder::vec3 calcedPosition ) const = 0;
    Type getType( ) const;
    cinder::vec3 const& getPosition( ) const;
    void update( cRigidBody* rigidbody );
    void setLeafs( std::vector<std::set<cColliderBase*>*>&& leafs );
    std::vector<std::set<cColliderBase*>*> const& getLeafs( ) const;
    void setPosition( cinder::vec3 const& position );
    void setLayer( unsigned int layer );
    unsigned int getLayer( ) const;
protected:
    unsigned int mLayer;
    Type mType;
    cinder::vec3 mPosition;
    std::vector<std::set<cColliderBase*>*> mLeafs;
};
}
