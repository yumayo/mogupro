#include <Node/Action/fade_in.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( fade_in, float duration )
{
    CREATE( fade_in, duration );
}
bool fade_in::init( float duration )
{
    finite_time_action::init( duration );
    return true;
}
void fade_in::setup( )
{
    _opacity = _target->get_opacity( );
}
void fade_in::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    auto const to = 1.0F;
    auto const from = _opacity;
    auto const temp = ease_liner( t, from, to );
    _target->set_opacity( temp );
}
}
}