#include <Node/Renderer/surface_cubic.h>
#include <cinder/gl/gl.h>
using namespace cinder;
namespace Node
{
namespace Renderer
{
CREATE_CPP( surface_cubic, cinder::vec2 size, cinder::ColorA color )
{
    CREATE( surface_cubic, size, color );
}
bool surface_cubic::init( cinder::vec2 size, cinder::ColorA color )
{
    return surface::init( size, color );
}
cinder::gl::Texture::Format surface_cubic::createSpriteFormat( )
{
    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    return format;
}
}
}