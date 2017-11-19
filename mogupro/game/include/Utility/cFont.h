#pragma once
#include <string>
#include <cinder/Vector.h>
#include <Node/Renderer/label.h>
namespace Utility
{
class cFont
{
public:
	void setup( std::string const& underAsset, float size );
	void setSize( float size );
	void draw( cinder::vec2 position, std::string const& utf8Str );
private:
	Node::Renderer::label label;
};
}
