#include <Node/Action/rotate_to.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( rotate_to, float duration, float radian )
{
    CREATE( rotate_to, duration, radian );
}
bool rotate_to::init( float duration, float radian )
{
    finite_time_action::init( duration );
    _rotation = radian;
    return true;
}
void rotate_to::setup( )
{
    _start_rotation = _target->get_rotation( );
}
void rotate_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    auto const to = _rotation;
    auto const from = _start_rotation;
    auto const temp = ease_liner( t, from, to );
    _target->set_rotation( temp );
}
void rotate_to::restart( )
{
    finite_time_action::restart( );
    setup( );
}
}
}