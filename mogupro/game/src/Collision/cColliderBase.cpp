#include <Collision/cColliderBase.h>
#include <Collision/cRigidBody.h>
#include <Collision/cCollisionManager.h>
namespace Collision
{
cColliderBase::cColliderBase( Type type, cinder::vec3 position )
    : mType( type )
    , mPosition( position )
    , mLayer( 0xFFFFFFFF )
{
}
cColliderBase::Type cColliderBase::getType( ) const
{
    return mType;
}
cinder::vec3 const & cColliderBase::getPosition( ) const
{
    return mPosition;
}
void cColliderBase::update( cRigidBody * rigidbody )
{
    cCollisionManager::getInstance( )->remove( *this );
    mPosition += rigidbody->getSpeed( );
    cCollisionManager::getInstance( )->add( *this );
}
void cColliderBase::setLeafs( std::vector<std::set<cColliderBase*>*>&& leafs )
{
    mLeafs = std::move( leafs );
}
std::vector<std::set<cColliderBase*>*> const & cColliderBase::getLeafs( ) const
{
    return mLeafs;
}
void cColliderBase::setPosition( cinder::vec3 const& position )
{
    cCollisionManager::getInstance( )->remove( *this );
    mPosition = position;
    cCollisionManager::getInstance( )->add( *this );
}
void cColliderBase::setLayer( unsigned int layer )
{
    mLayer = layer;
}
unsigned int cColliderBase::getLayer( ) const
{
    return mLayer;
}
}
