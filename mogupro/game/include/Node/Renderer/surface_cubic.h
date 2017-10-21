#pragma once
#include <Node/Renderer/surface.h>
namespace Node
{
namespace Renderer
{
class surface_cubic : public surface
{
public:
    CREATE_H( surface_cubic, cinder::vec2 size, cinder::ColorA color = cinder::ColorA( 1, 1, 1, 1 ) );
    bool init( cinder::vec2 size, cinder::ColorA color );
protected:
    virtual cinder::gl::Texture::Format createSpriteFormat( );
};
}
}