#include <Node/Renderer/letter.h>
#include <cinder/gl/scoped.h>
#include <Node/Library/font.h>
using namespace cinder;
namespace Node
{
namespace Renderer
{
struct letter::member
{
	Library::font font = { 2048, 2048, FONS_ZERO_TOPLEFT };
	int id = FONS_INVALID;
};
const char* letter::_vertex_shader =
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
const char* letter::_fragment_shader =
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
CREATE_CPP( letter, std::string const& relative_path, float size )
{
	CREATE( letter, relative_path, size );
}
bool letter::init( std::string const& relative_path, float size )
{
	// assert_log( !app::getAssetPath( relative_path ).empty( ), "ファイルが見つかりません。", return false );

	_m = std::make_shared<letter::member>( );

	set_anchor_point( { 0.5F, 0.5F } );
	set_pivot( { 0.5F, 0.5F } );
	_font_shader = ci::gl::GlslProg::create( _vertex_shader, _fragment_shader );
	_relative_path = relative_path;
	_m->id = fonsAddFont( _m->font, relative_path.c_str( ), app::getAssetPath( relative_path ).string( ).c_str( ) );
	set_size( size );
	set_color( _color );

	return true;
}
void letter::render( )
{
	gl::ScopedGlslProg glsl( _font_shader );
	int index = 0;
	for ( auto& text : _text )
	{
		auto position = vec2( index * ( _size + _space ), 0 );
		position += vec2( _size, _size ) * get_anchor_point( );
		position -= _text_content_map[index] * get_anchor_point( );
		position.y += _text_content_map[index].y;
		fonsDrawText( _m->font, position.x, position.y, text.c_str( ), nullptr );
		index++;
	}
}
void letter::set_space( float value )
{
	_space = value;
}
void letter::set_size( float value )
{
	_size = value;
	fonsSetSize( _m->font, _size );
}
float const & letter::get_size( )
{
	return _size;
}
void letter::set_text( std::string const & value )
{
	_text.set_string( value );
	float w = _text.size( ) * _size + (_text.size( ) - 1) * _space;
	float h = _size;
	_content_size = vec2( w, h );

	_text_content_map.clear( );

	for ( auto& text : _text )
	{
		float bounds[4];
		fonsTextBounds( _m->font, 0, 0, text.c_str( ), nullptr, bounds );
		int w = (int)( bounds[2] - bounds[0] );
		int h = (int)( bounds[3] - bounds[1] );
		_text_content_map.emplace_back( w, h );
	}
}
std::string const & letter::get_text( )
{
	return _text.c_str( );
}
void letter::set_color( cinder::ColorA const& value )
{
	node::set_color( value );
	fonsSetColor( _m->font, Library::font::color( _color ) );
}
}
}