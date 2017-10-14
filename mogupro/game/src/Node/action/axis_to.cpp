#include <Node/Action/axis_to.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( axis_to, float duration, cinder::vec3 axis )
{
    CREATE( axis_to, duration, axis );
}
bool axis_to::init( float duration, cinder::vec3 axis )
{
    finite_time_action::init( duration );
    _axis = axis;
    return true;
}
void axis_to::setup( )
{
    _start_axis = _target->get_axis( );
}
void axis_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    auto const to = _axis;
    auto const from = _start_axis;
    auto const temp = vec3( ease_liner( t, from.x, to.x ), ease_liner( t, from.y, to.y ), ease_liner( t, from.z, to.z ) );
    _target->set_axis( temp );
}
void axis_to::restart( )
{
    finite_time_action::restart( );
    setup( );
}
}
}