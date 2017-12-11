#include <Node/Renderer/sprite_cubic_animation.h>
#include <cinder/gl/gl.h>
using namespace cinder;
namespace Node
{
namespace Renderer
{
CREATE_CPP( sprite_cubic_animation, std::string const& relative_path )
{
    CREATE( sprite_cubic_animation, relative_path );
}
bool sprite_cubic_animation::init( std::string const& relative_path )
{
    return sprite_animation::init( relative_path );
}
cinder::gl::Texture::Format sprite_cubic_animation::createSpriteFormat( )
{
    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    return format;
}
}
}