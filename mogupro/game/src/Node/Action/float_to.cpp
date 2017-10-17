#include <Node/Action/float_to.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( float_to, float duration, float from, float to, std::function<void( float value )> callback )
{
    CREATE( float_to, duration, from, to, callback );
}
bool float_to::init( float duration, float from, float to, std::function<void( float value )> callback )
{
    finite_time_action::init( duration );
    _from = from;
    _to = to;
    _callback = callback;
    return true;
}
void float_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    if ( _callback ) _callback( ease_liner( t, _from, _to ) );
}
}
}