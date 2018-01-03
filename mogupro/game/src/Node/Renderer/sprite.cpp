#include <Node/Renderer/sprite.h>
#include <cinder/gl/gl.h>
using namespace cinder;
namespace Node
{
namespace Renderer
{
CREATE_CPP( sprite, std::string const& relative_path )
{
    CREATE( sprite, relative_path );
}
CREATE_CPP( sprite, cinder::gl::TextureRef handle )
{
	CREATE( sprite, handle );
}
bool sprite::init( std::string const& relative_path )
{
    // assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    return init( gl::Texture::create( loadImage( app::loadAsset( relative_path ) ), createSpriteFormat( ) ) );
}
bool sprite::init( cinder::gl::TextureRef handle )
{
	set_anchor_point( { 0.5F, 0.5F } );
	set_pivot( { 0.5F, 0.5F } );

	_texture = handle;
	_content_size = _texture->getSize( );

	return true;
}
void sprite::render( )
{
    gl::draw( _texture, Rectf( vec2( 0 ), _content_size ) );
}
cinder::gl::Texture::Format sprite::createSpriteFormat( )
{
    gl::Texture::Format format;
    return format;
}
}
}