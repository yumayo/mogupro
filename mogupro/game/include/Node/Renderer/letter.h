#pragma once
#include <Node/node.h>
#include <cinder/gl/GlslProg.h>
#include <Utility/utf8.h>
namespace Node
{
namespace Renderer
{
class letter : public node
{
	struct member;
	hardptr<member> _m;
public:
	CREATE_H( letter, std::string const& relative_path, float size );
	bool init( std::string const& relative_path, float size );
	void render( ) override;
public:
	void set_space( float value );
	void set_size( float value );
	float const& get_size( );
	void set_text( std::string const& value );
	std::string const& get_text( );
	void set_color( cinder::ColorA const& value ) override;
private:
	static const char* _vertex_shader;
	static const char* _fragment_shader;
	ci::gl::GlslProgRef _font_shader;
	std::vector<cinder::vec2> _text_content_map;
	Utility::utf8 _text;
	std::string _relative_path;
	float _size = 0.0F;
	float _space = 0.0F;
};
}
}
