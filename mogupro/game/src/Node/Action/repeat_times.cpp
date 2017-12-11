#include <Node/Action/repeat_times.h>
namespace Node
{
namespace Action
{
CREATE_CPP( repeat_times, hardptr<finite_time_action> const& time_action, int number_of_times )
{
    CREATE( repeat_times, time_action, number_of_times );
}
bool repeat_times::init( hardptr<finite_time_action> const& time_action, int number_of_times )
{
    repeat_forever::init( time_action );
    _init_number_of_times = number_of_times;
    _number_of_times = _init_number_of_times;
    return true;
}
bool repeat_times::is_done( )
{
    return _number_of_times <= 0 && _time_action->is_done( );
}
float repeat_times::update( float delta )
{
begin:
    delta = _time_action->update( delta );
    if ( _time_action->is_done( ) )
    {
        _number_of_times -= 1;
        if ( !is_done( ) )
        {
            setup( );
            _time_action->restart( );
            goto begin;
        }
    }
    return delta;
}
void repeat_times::restart( )
{
    finite_time_action::restart( );
    _number_of_times = _init_number_of_times;
    _time_action->restart( );
}
}
}