#include <Node/Action/move_to_target.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( move_to_target, float duration, hardptr<node> const& target )
{
    CREATE( move_to_target, duration, target );
}
bool move_to_target::init( float duration, hardptr<node> const& target )
{
    finite_time_action::init( duration );
    _new_target = target;
    return true;
}
void move_to_target::setup( )
{
    _start_position = _target->get_position_3d( );
}
void move_to_target::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    auto const to = _new_target->get_position_3d( );
    auto const from = _start_position;
    auto const temp = vec3( ease_liner( t, from.x, to.x ), ease_liner( t, from.y, to.y ), ease_liner( t, from.z, to.z ) );
    _target->set_position_3d( temp );
}
void move_to_target::restart( )
{
    finite_time_action::restart( );
    setup( );
}
}
}