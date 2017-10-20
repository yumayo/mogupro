#include <Node/Renderer/rect_edge.h>
#include <cinder/gl/gl.h>
using namespace cinder;
namespace Node
{
namespace Renderer
{
CREATE_CPP( rect_edge, cinder::vec2 const & size, float line_width )
{
    CREATE( rect_edge, size, line_width );
}
bool rect_edge::init( cinder::vec2 const& size, float line_width )
{
    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    set_content_size( size );
    _line_width = line_width;
    return true;
}
void rect_edge::render( )
{
    gl::lineWidth( _line_width );
    gl::drawStrokedRect( Rectf( vec2( 0 ), _content_size ) );
    gl::lineWidth( 1.0F );
}
}
}