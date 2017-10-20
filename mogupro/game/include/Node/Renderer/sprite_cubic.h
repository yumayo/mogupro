#pragma once
#include <Node/Renderer/sprite.h>
namespace Node
{
namespace Renderer
{
class sprite_cubic : public sprite
{
public:
    CREATE_H( sprite_cubic, std::string const& relative_path );
    bool init( std::string const& relative_path );
protected:
    cinder::gl::Texture::Format createSpriteFormat( ) override;
};
}
}