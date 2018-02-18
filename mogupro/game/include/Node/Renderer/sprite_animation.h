#pragma once
#include <Node/node.h>
#include <cinder/gl/Texture.h>
namespace Node
{
namespace Renderer
{
class sprite_animation : public node
{
public:
    CREATE_H( sprite_animation, std::string const& relative_path );
	CREATE_H(sprite_animation, cinder::gl::TextureRef const& texture);
    virtual ~sprite_animation( );
    bool init( std::string const& relative_path );
	bool init(cinder::gl::TextureRef const& texture);
    void update( float delta ) override;
    void render( ) override;
public:
    cinder::vec2 get_cut_size( );
    void set_cut_size( cinder::vec2 value );
    int get_cut_x( );
    void set_cut_x( int value );
    int get_cut_y( );
    void set_cut_y( int value );
    cinder::ivec2 get_cut( );
    void set_cut( cinder::ivec2 value );
    float get_framerate( );
    void set_framerate( float value );
protected:
    virtual cinder::gl::Texture::Format createSpriteFormat( );
    cinder::gl::TextureRef _texture;
    cinder::vec2 _cut_size;
    int _cut_x = 0;
    int _cut_y = 0;
    float _framerate = 24.0F;
    float _animation_time = 0.0F;
};
}
}