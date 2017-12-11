#include <Node/Action/finite_time_action.h>
#include <Node/node.h>
#include <algorithm>
namespace Node
{
namespace Action
{
CREATE_CPP( finite_time_action, float duration )
{
    CREATE( finite_time_action, duration );
}
bool finite_time_action::init( float duration )
{
    _duration = duration;
    return true;
}
float finite_time_action::update( float delta )
{
    auto overflow_second = set_time( _time + delta );
    step( _time / _duration );
    return overflow_second;
}
void finite_time_action::restart( )
{
    _time = 0.0F;
}
float finite_time_action::set_time( float value )
{
    _time = value;
    auto overflow_second = std::max( _time - _duration, 0.0F );
    _time = std::min( _time, _duration );
    return overflow_second;
}
float finite_time_action::get_time( )
{
    return _time;
}
void finite_time_action::set_duration( float value )
{
    _duration = value;
}
float finite_time_action::get_duration( )
{
    return _duration;
}
bool finite_time_action::is_done( )
{
    return _duration <= _time;
}
}
}