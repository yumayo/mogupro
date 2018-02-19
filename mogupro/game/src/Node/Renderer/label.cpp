#include <Node/Renderer/label.h>
#include <cinder/gl/scoped.h>
#include <Node/Library/font.h>
using namespace cinder;
namespace Node
{
namespace Renderer
{
struct label::member
{
    Library::font font = { 2048, 2048, FONS_ZERO_TOPLEFT };
    int id = FONS_INVALID;
};
const char* label::_vertex_shader =
"#version 330\n"
"\n"
"uniform mat4 ciModelViewProjection;\n"
"\n"
"in vec4 ciPosition;\n"
"in vec4 ciColor;\n"
"in vec2 ciTexCoord0;\n"
"\n"
"out vec2 TexCoord0;\n"
"out vec4 Color;\n"
"\n"
"\n"
"void main( void ) {\n"
"    gl_Position = ciModelViewProjection * ciPosition;\n"
"    TexCoord0 = ciTexCoord0;\n"
"    Color = ciColor;\n"
"}\n";
const char* label::_fragment_shader =
"#version 330\n"
"\n"
"uniform sampler2D	uTex0;\n"
"\n"
"in vec4 Color;\n"
"in vec2 TexCoord0;\n"
"\n"
"out vec4 oColor;\n"
"\n"
"\n"
"void main( void ) {\n"
"    oColor = vec4( 1, 1, 1, texture( uTex0, TexCoord0 ).r ) * Color;\n"
"}\n";
CREATE_CPP( label, std::string const& relative_path, float size )
{
    CREATE( label, relative_path, size );
}
bool label::init( std::string const& relative_path, float size )
{
    // assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

    _m = std::make_shared<label::member>( );

    set_anchor_point( { 0.5F, 0.5F } );
    set_pivot( { 0.5F, 0.5F } );
    _font_shader = ci::gl::GlslProg::create( _vertex_shader, _fragment_shader );
    _relative_path = relative_path;
    _m->id = fonsAddFont( _m->font, relative_path.c_str( ), app::getAssetPath( relative_path ).string( ).c_str( ) );
    set_size( size );
    set_color( _color );

    return true;
}
void label::render( )
{
    gl::ScopedGlslProg glsl( _font_shader );
    fonsDrawText( _m->font, 0, -_height, _text.c_str( ), nullptr );
}
void label::set_size( float value )
{
    _size = value;
    fonsSetSize( _m->font, _size );
}
float const & label::get_size( )
{
    return _size;
}
void label::set_text( std::string const & value )
{
    _text = value;
    float bounds[4];
    fonsTextBounds( _m->font, 0, 0, _text.c_str( ), nullptr, bounds );
    int w = (int)(bounds[2] - bounds[0]);
    int h = (int)(bounds[3] - bounds[1]);
    _height = bounds[3] + bounds[1];
    _content_size = vec2( w, h );
}
std::string const & label::get_text( )
{
    return _text;
}
void label::set_color( cinder::ColorA const& value )
{
    node::set_color( value );
    fonsSetColor( _m->font, Library::font::color( _color ) );
}
void label::set_opacity(float value)
{
	node::set_opacity(value);
	fonsSetColor(_m->font, Library::font::color(_color));
}
}
}