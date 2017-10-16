#include <Collision/cColliderBase.h>
#include <Collision/cRigidBody.h>
#include <Collision/cCollisionManager.h>
namespace Collision
{
cColliderBase::cColliderBase( Type type, cinder::vec3 position )
    : mType( type )
    , mPosition( position )
{
}
cColliderBase::Type cColliderBase::getType( )
{
    return mType;
}
cinder::vec3 const & cColliderBase::getPosition( )
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
std::vector<std::set<cColliderBase*>*> const & cColliderBase::getLeafs( )
{
    return mLeafs;
}
void cColliderBase::calc( cRigidBody * rigidbody, cinder::vec3 const & position )
{
    mPosition = position;
    rigidbody->setSpeed( rigidbody->getSpeed( ) * 0.1F );
}
}
