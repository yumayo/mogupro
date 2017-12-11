#include <Node/Action/opacity_to.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( opacity_to, float duration, float opacity )
{
    CREATE( opacity_to, duration, opacity );
}
bool opacity_to::init( float duration, float opacity )
{
    finite_time_action::init( duration );
    _opacity = opacity;
    return true;
}
void opacity_to::setup( )
{
    _start_opacity = _target->get_opacity( );
}
void opacity_to::step( float t )
{
    t = clamp( t, 0.0F, 1.0F );
    auto const to = _opacity;
    auto const from = _start_opacity;
    auto const temp = ease_liner( t, from, to );
    _target->set_opacity( temp );
}
void opacity_to::restart( )
{
    finite_time_action::restart( );
    setup( );
}
}
}