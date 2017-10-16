#include <Node/Action/move_to.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( move_to, float duration, cinder::vec2 position )
{
    CREATE( move_to, duration, position );
}
bool move_to::init( float duration, cinder::vec2 position )
{
    finite_time_action::init( duration );
    _position = vec3( position, 0.0F );
    _is_2d = true;
    return true;
}
CREATE_CPP( move_to, float duration, cinder::vec3 position )
{
    CREATE( move_to, duration, position );
}
bool move_to::init( float duration, cinder::vec3 position_3d )
{
    finite_time_action::init( duration );
    _position = position_3d;
    _is_2d = false;
    return true;
}
void move_to::setup( )
{
    _start_position = _target->get_position_3d( );
    if ( _is_2d ) _position.z = _start_position.z;
}
void move_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    auto const to = _position;
    auto const from = _start_position;
    auto const temp = vec3( ease_liner( t, from.x, to.x ), ease_liner( t, from.y, to.y ), ease_liner( t, from.z, to.z ) );
    _target->set_position_3d( temp );
}
void move_to::restart( )
{
    finite_time_action::restart( );
    setup( );
}
}
}