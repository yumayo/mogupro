#pragma once
#include <Node/node.h>
#include <cinder/gl/Texture.h>
namespace Node
{
namespace Renderer
{
class sprite : public node
{
public:
    virtual ~sprite( ) { }
	CREATE_H( sprite, std::string const& relative_path );
	bool init( std::string const& relative_path );
	CREATE_H( sprite, cinder::gl::TextureRef handle );
	bool init( cinder::gl::TextureRef handle );
    virtual void render( ) override;
protected:
    virtual cinder::gl::Texture::Format createSpriteFormat( );
    cinder::gl::TextureRef _texture;
};
}
}