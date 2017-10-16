#include <Node/Action/scale_by.h>
#include <Node/node.h>
using namespace cinder;
namespace Node
{
namespace Action
{
CREATE_CPP( scale_by, float duration, cinder::vec2 scale )
{
    CREATE( scale_by, duration, scale );
}
bool scale_by::init( float duration, cinder::vec2 scale )
{
    scale_to::init( duration, scale );
    _init_scale = vec3( scale, 0.0F ); // 2dの場合はスケールの上下はないので0.0Fです。
    return true;
}
CREATE_CPP( scale_by, float duration, cinder::vec3 scale )
{
    CREATE( scale_by, duration, scale );
}
bool scale_by::init( float duration, cinder::vec3 scale )
{
    scale_to::init( duration, scale );
    _init_scale = scale;
    return true;
}
void scale_by::setup( )
{
    scale_to::setup( );
    _scale = _init_scale + _start_scale;
}
void scale_by::restart( )
{
    scale_to::restart( );
    setup( );
}
}
}