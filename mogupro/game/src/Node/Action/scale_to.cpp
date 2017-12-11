#include <Node/Action/scale_to.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( scale_to, float duration, cinder::vec2 scale )
{
    CREATE( scale_to, duration, scale );
}
bool scale_to::init( float duration, cinder::vec2 scale )
{
    finite_time_action::init( duration );
    _scale = vec3( scale, 0.0F );
    _is_2d = true;
    return true;
}
CREATE_CPP( scale_to, float duration, cinder::vec3 scale )
{
    CREATE( scale_to, duration, scale );
}
bool scale_to::init( float duration, cinder::vec3 scale )
{
    finite_time_action::init( duration );
    _scale = scale;
    _is_2d = false;
    return true;
}
void scale_to::setup( )
{
    _start_scale = _target->get_scale_3d( );
    if ( _is_2d ) _scale.z = _start_scale.z;
}
void scale_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    auto const to = _scale;
    auto const from = _start_scale;
    auto const temp = vec3( ease_liner( t, from.x, to.x ), ease_liner( t, from.y, to.y ), ease_liner( t, from.z, to.z ) );
    _target->set_scale_3d( temp );
}
void scale_to::restart( )
{
    finite_time_action::restart( );
    setup( );
}
}
}