#include <Node/Action/repeat_forever.h>
namespace Node
{
namespace Action
{
CREATE_CPP( repeat_forever, hardptr<finite_time_action> const& time_action )
{
    CREATE( repeat_forever, time_action );
}
bool repeat_forever::init( hardptr<finite_time_action> const& time_action )
{
    _time_action = time_action;
    return true;
}
void repeat_forever::setup( )
{
    _time_action->setup( _target, _pause );
}
bool repeat_forever::is_done( )
{
    // 無限ループを作成するクラスなので。
    return false;
}
float repeat_forever::update( float delta )
{
begin:
    delta = _time_action->update( delta );
    if ( _time_action->is_done( ) )
    {
        setup( );
        _time_action->restart( );
        goto begin;
    }
    return delta;
}
void repeat_forever::restart( )
{
    finite_time_action::restart( );
    _time_action->restart( );
}
}
}