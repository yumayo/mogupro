#pragma once
#include <Node/Renderer/sprite_animation.h>
namespace Node
{
namespace Renderer
{
class sprite_cubic_animation : public sprite_animation
{
public:
    CREATE_H( sprite_cubic_animation, std::string const& relative_path );
    bool init( std::string const& relative_path );
protected:
    cinder::gl::Texture::Format createSpriteFormat( ) override;
};
}
}