#pragma once
#include <cinder/Vector.h>
#include <Collision/cAABBCollider.h>
namespace Collision
{
class cStaticBlockSimple
{
public:
    cStaticBlockSimple( cinder::vec3 pos, cinder::vec3 size );
    void draw( );
private:
    Collision::cAABBCollider mCollider;
};
}
