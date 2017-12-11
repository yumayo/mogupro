#include <Node/Action/rotate_by.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( rotate_by, float duration, float radian )
{
    CREATE( rotate_by, duration, radian );
}
bool rotate_by::init( float duration, float radian )
{
    rotate_to::init( duration, radian );
    _init_rotation = radian;
    return true;
}
void rotate_by::setup( )
{
    rotate_to::setup( );
    _rotation = _init_rotation + _start_rotation;
}
void rotate_by::restart( )
{
    rotate_to::restart( );
    setup( );
}
}
}