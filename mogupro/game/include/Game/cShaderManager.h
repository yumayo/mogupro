#pragma once
#include <Utility/cSingletonAble.h>
#include <cinder/gl/GlslProg.h>
#include <cinder/gl/Texture.h>
#include <cinder/Camera.h>
#include <cinder/gl/Fbo.h>
#include <cinder/gl/Ubo.h>
#include <Game/Light/LightDefine.h>
namespace Game
{
class cShaderManager : public Utility::cSingletonAble<cShaderManager>
{
public:
	cShaderManager( ) { }
	~cShaderManager( ) { }
	void setup( );
	void uniformUpdate( );
	void uniformUpdate( int chunkId );
	void uniformUpdate(std::vector<int> const& pointLightIds, std::vector<int> const& lineLightIds, std::vector<int> const& spotLightIds );
	void update( std::function<void( )> const& drawFunc );
	void draw( std::function<void( )> const& render );
private:
	cinder::gl::Texture2dRef mShadowTex;
	cinder::CameraOrtho mCamera;
	cinder::gl::FboRef mFbo;
	cinder::gl::GlslProgRef mGlsl;
	cinder::gl::UboRef mPointLightUBO;
	struct
	{
		cinder::vec4 useIndices[Light::MAX_POINT_LIGHT_NUM / 4U];
		cinder::vec4 modelViewPositions[Light::MAX_POINT_LIGHT_NUM];
		cinder::vec4 colorWithRadiuses[Light::MAX_POINT_LIGHT_NUM];
	}mPointLightParams;
	cinder::gl::UboRef mLineLightUBO;
	struct
	{
		cinder::vec4 useIndices[Light::MAX_LINE_LIGHT_NUM / 4U];
		cinder::vec4 modelViewPositionsA[Light::MAX_LINE_LIGHT_NUM];
		cinder::vec4 modelViewPositionsB[Light::MAX_LINE_LIGHT_NUM];
		cinder::vec4 colorWithRadiuses[Light::MAX_LINE_LIGHT_NUM];
	}mLineLightParams;
	cinder::gl::UboRef mSpotLightUBO;
	struct
	{
		cinder::vec4 useIndices[Light::MAX_SPOT_LIGHT_NUM / 4U];
		cinder::vec4 modelViewPositionsA[Light::MAX_SPOT_LIGHT_NUM];
		cinder::vec4 modelViewPositionsB[Light::MAX_SPOT_LIGHT_NUM];
		cinder::vec4 colorWithRadiuses[Light::MAX_SPOT_LIGHT_NUM];
	}mSpotLightParams;
	bool mUseShadow = true;
	bool mUseAllLight = true;
};
}
