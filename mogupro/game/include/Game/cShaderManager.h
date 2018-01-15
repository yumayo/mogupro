#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/gl/GlslProg.h>
#include <cinder/gl/Texture.h>
#include <cinder/Camera.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/Ubo.h>
namespace Game
{
class cShaderManager : public Utility::cSingletonAble<cShaderManager>
{
public:
	cShaderManager( ) { }
	~cShaderManager( ) { }
	void setup( bool useShadow );
	void uniformUpdate( );
	void uniformUpdate( int chunkId );
	void update( std::function<void( )> const& drawFunc );
	void draw( std::function<void( )> const& render );
private:
	cinder::gl::Texture2dRef mShadowTex;
	cinder::CameraOrtho mCamera;
	cinder::gl::FboRef mFbo;
	cinder::gl::GlslProgRef mGlsl;
	cinder::gl::Texture2dRef mPointLightTex;
	cinder::Surface32f mPointLightSurface;
	cinder::gl::UboRef mPointLightUBO;
	struct PointLightParams
	{
		cinder::vec3 uPointLightModelViewPositions[100];
		cinder::vec3 uPointLightColors[100];
		float uPointLightRadiuses[100];
	};
	bool mUseShadow = false;
};
}
