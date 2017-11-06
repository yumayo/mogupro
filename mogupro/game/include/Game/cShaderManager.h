#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/gl/GlslProg.h>
#include <cinder/gl/Texture.h>
#include <cinder/Camera.h>
#include <cinder/gl/Fbo.h>
namespace Game
{
class cShaderManager : public Utility::cSingletonAble<cShaderManager>
{
public:
	cShaderManager( ) { }
	~cShaderManager( ) { }
	void setup( );
	void update( std::function<void( )> const& drawFunc );
	void draw( std::function<void()>  const& render );
private:
	cinder::gl::Texture2dRef mShadowTex;
	cinder::CameraOrtho mCamera;
	cinder::gl::FboRef mFbo;
	cinder::gl::GlslProgRef mGlsl;
};
}
