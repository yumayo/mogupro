#include <Collision/cColliderBase.h>
namespace Collision
{
cColliderBase::cColliderBase( Type type, cinder::vec3 position )
    : mType( type )
    , mPosition( position )
{
}
}
