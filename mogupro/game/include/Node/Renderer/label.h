#pragma once
#include <Node/node.h>
#include <cinder/gl/GlslProg.h>
namespace Node
{
namespace Renderer
{
class label : public node
{
    struct member;
    hardptr<member> _m;
public:
    CREATE_H( label, std::string const& relative_path, float size );
    bool init( std::string const& relative_path, float size );
    void render( ) override;
public:
    void set_size( float value );
    float const& get_size( );
    void set_text( std::string const& value );
    std::string const& get_text( );
    void set_color( cinder::ColorA const& value ) override;
	void set_opacity( float value ) override;
private:
    static const char* _vertex_shader;
    static const char* _fragment_shader;
    ci::gl::GlslProgRef _font_shader;
    std::string _text;
    std::string _relative_path;
    float _size = 0.0F;
    float _height = 0.0F;
};
}
}