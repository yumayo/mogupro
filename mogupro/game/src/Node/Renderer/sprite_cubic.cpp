#include <Node/Renderer/sprite_cubic.h>
#include <cinder/gl/gl.h>
using namespace cinder;
namespace Node
{
namespace Renderer
{
CREATE_CPP( sprite_cubic, std::string const& relative_path )
{
    CREATE( sprite_cubic, relative_path );
}
bool sprite_cubic::init( std::string const& relative_path )
{
    return sprite::init( relative_path );
}
cinder::gl::Texture::Format sprite_cubic::createSpriteFormat( )
{
    gl::Texture::Format format;
    format.setMinFilter( GL_NEAREST );
    format.setMagFilter( GL_NEAREST );
    return format;
}
}
}