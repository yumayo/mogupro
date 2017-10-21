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
    CREATE_H( sprite, std::string const& relative_path );
    virtual ~sprite( ) { }
    bool init( std::string const& relative_path );
    virtual void render( ) override;
protected:
    virtual cinder::gl::Texture::Format createSpriteFormat( );
    cinder::gl::TextureRef _texture;
};
}
}