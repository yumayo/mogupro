#include <Node/Action/axis_by.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( axis_by, float duration, cinder::vec3 axis )
{
    CREATE( axis_by, duration, axis );
}
bool axis_by::init( float duration, cinder::vec3 axis )
{
    axis_to::init( duration, axis );
    _init_axis = axis;
    return true;
}
void axis_by::setup( )
{
    axis_to::setup( );
    _axis = _init_axis + _start_axis;
}
void axis_by::restart( )
{
    axis_to::restart( );
    setup( );
}
}
}