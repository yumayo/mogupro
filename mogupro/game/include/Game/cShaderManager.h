#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/gl/GlslProg.h>
namespace Game
{
class cShaderManager : public Utility::cSingletonAble<cShaderManager>
{
	cinder::gl::GlslProgRef mGlsl;
public:
	cShaderManager( ) { }
	~cShaderManager( ) { }
	void setup( );
	void draw( std::function<void()> render );
};
}
