#pragma once
#include <cinder/Vector.h>
#include <Collision/cAABBCollider.h>
#include <Utility/cUserPointer.hpp>
namespace Collision
{
class cStaticBlockSimple
{
public:
    cStaticBlockSimple( cinder::vec3 pos, cinder::vec3 size );
    void draw( );
private:
    Utility::hardptr<Collision::cAABBCollider> mCollider;
};
}
