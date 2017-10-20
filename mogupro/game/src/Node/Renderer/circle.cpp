#include <Node/Renderer/circle.h>
#include <cinder/gl/gl.h>
using namespace cinder;
namespace Node
{
namespace Renderer
{
CREATE_CPP( circle, float radius, int segments )
{
    CREATE( circle, radius, segments );
}
circle::~circle( )
{
}
bool circle::init( float radius, int segments )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    set_radius( radius );
    set_segments( segments );
    return true;
}
void circle::render( )
{
    gl::drawSolidCircle( vec2( _radius, _radius ), _radius, _segments );
}
void circle::set_radius( float value )
{
    _radius = value;
    _content_size = { _radius * 2, _radius * 2 };
}
float circle::get_radius( )
{
    return _radius;
}
void circle::set_segments( int value )
{
    _segments = value;
}
int circle::get_segments( )
{
    return _segments;
}
}
}