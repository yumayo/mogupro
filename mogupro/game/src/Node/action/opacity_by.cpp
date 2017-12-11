#include <Node/Action/opacity_by.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( opacity_by, float duration, float opacity )
{
    CREATE( opacity_by, duration, opacity );
}
bool opacity_by::init( float duration, float opacity )
{
    opacity_to::init( duration, opacity );
    _init_opacity = opacity;
    return true;
}
void opacity_by::setup( )
{
    opacity_to::setup( );
    _opacity = _init_opacity + _start_opacity;
}
void opacity_by::restart( )
{
    opacity_to::restart( );
    setup( );
}
}
}