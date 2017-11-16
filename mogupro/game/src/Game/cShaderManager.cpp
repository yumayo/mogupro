#include <Game/cShaderManager.h>
#include <cinder/gl/gl.h>
#include <Game/cGemManager.h>
#include <CameraManager/cCameraManager.h>
#include <Game/cPlayerManager.h>
#include <Game/cLightManager.h>
using namespace ci;
namespace Game
{
void cShaderManager::setup( )
{
	int scale = 8192;
	gl::Texture2d::Format depthFormat;
	depthFormat.setInternalFormat( GL_DEPTH_COMPONENT32F );
	depthFormat.setCompareMode( GL_COMPARE_REF_TO_TEXTURE );
	depthFormat.setMagFilter( GL_LINEAR );
	depthFormat.setMinFilter( GL_LINEAR );
	depthFormat.setWrap( GL_CLAMP_TO_BORDER, GL_CLAMP_TO_BORDER );
	depthFormat.setBorderColor( ColorA( 1, 1, 1, 1 ) );
	depthFormat.setCompareFunc( GL_LEQUAL );
	mShadowTex = gl::Texture2d::create( scale, scale, depthFormat );

	gl::Fbo::Format fboFormat;
	fboFormat.attachment( GL_DEPTH_ATTACHMENT, mShadowTex );
	mFbo = gl::Fbo::create( mShadowTex->getWidth( ), mShadowTex->getHeight( ), fboFormat );

	mGlsl = gl::GlslProg::create( app::loadAsset( "Shader/world.vert" ),
								  app::loadAsset( "Shader/world.frag" ) );

	int size = 128;
	vec3 pos( 80, 80, 80 );
	mCamera.setOrtho( -size, size, -size, size, 0.25F, 512.0F );
	mCamera.lookAt( pos + vec3( size, size, -size ), pos + vec3( 0, 0, 0 ) );
}
void cShaderManager::uniformUpdate( )
{
	{
		// ポイントライトのデータを送ります。
		auto lights = Game::cLightManager::getInstance( )->getPointLights( );
		{
			std::vector<vec3> positions;
			std::vector<vec3> colors;
			std::vector<float> radiuses;
			int lightNum = std::min( lights.size( ), 100U );
			for ( auto& light : lights )
			{
				positions.emplace_back( vec3( CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light->getPosition( ), 1 ) ) );
				colors.emplace_back( light->color );
				radiuses.emplace_back( light->getRadius( ) );
			}
			mGlsl->uniform( "uPointLineNum", lightNum );
			mGlsl->uniform( "uModelViewPointLightPositions", positions.data( ), lightNum );
			mGlsl->uniform( "uModelViewPointLightColors", colors.data( ), lightNum );
			mGlsl->uniform( "uModelViewPointLightRadiuses", radiuses.data( ), lightNum );
		}
	}
	{
		// ラインライトの情報を送ります。
		auto lights = Game::cLightManager::getInstance( )->getLineLights( );
		{
			std::vector<vec3> beginPositions;
			std::vector<vec3> endPositions;
			std::vector<vec3> colors;
			std::vector<float> radiuses;
			int lightNum = std::min( lights.size( ), 100U );
			for ( auto& light : lights )
			{
				beginPositions.emplace_back( vec3( CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light->getBeginPosition( ), 1 ) ) );
				endPositions.emplace_back( vec3( CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light->getEndPosition( ), 1 ) ) );
				colors.emplace_back( light->color );
				radiuses.emplace_back( light->getRadius( ) );
			}
			mGlsl->uniform( "uLineLightNum", lightNum );
			mGlsl->uniform( "uModelViewLineLightPositionsA", beginPositions.data( ), lightNum );
			mGlsl->uniform( "uModelViewLineLightPositionsB", endPositions.data( ), lightNum );
			mGlsl->uniform( "uModelViewLineLightColors", colors.data( ), lightNum );
			mGlsl->uniform( "uModelViewLineLightRadiuses", radiuses.data( ), lightNum );
		}
	}
}
void cShaderManager::uniformUpdate( int chunkId )
{
	{
		// ポイントライトのデータを送ります。
		auto lights = Game::cLightManager::getInstance( )->getPointLights( chunkId );
		if ( lights )
		{
			std::vector<vec3> positions;
			std::vector<vec3> colors;
			std::vector<float> radiuses;
			int lightNum = std::min( lights->size( ), 100U );
			for ( auto& light : *lights )
			{
				positions.emplace_back( vec3( CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light->getPosition( ), 1 ) ) );
				colors.emplace_back( light->color );
				radiuses.emplace_back( light->getRadius( ) );
			}
			mGlsl->uniform( "uPointLineNum", lightNum );
			mGlsl->uniform( "uModelViewPointLightPositions", positions.data( ), lightNum );
			mGlsl->uniform( "uModelViewPointLightColors", colors.data( ), lightNum );
			mGlsl->uniform( "uModelViewPointLightRadiuses", radiuses.data( ), lightNum );
		}
	}
	{
		// ラインライトの情報を送ります。
		auto lights = Game::cLightManager::getInstance( )->getLineLights( chunkId );
		if ( lights )
		{
			std::vector<vec3> beginPositions;
			std::vector<vec3> endPositions;
			std::vector<vec3> colors;
			std::vector<float> radiuses;
			int lightNum = std::min( lights->size( ), 100U );
			for ( auto& light : *lights )
			{
				beginPositions.emplace_back( vec3( CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light->getBeginPosition( ), 1 ) ) );
				endPositions.emplace_back( vec3( CAMERA->getCamera( ).getViewMatrix( ) * ci::vec4( light->getEndPosition( ), 1 ) ) );
				colors.emplace_back( light->color );
				radiuses.emplace_back( light->getRadius( ) );
			}
			mGlsl->uniform( "uLineLightNum", lightNum );
			mGlsl->uniform( "uModelViewLineLightPositionsA", beginPositions.data( ), lightNum );
			mGlsl->uniform( "uModelViewLineLightPositionsB", endPositions.data( ), lightNum );
			mGlsl->uniform( "uModelViewLineLightColors", colors.data( ), lightNum );
			mGlsl->uniform( "uModelViewLineLightRadiuses", radiuses.data( ), lightNum );
		}
	}
}
void cShaderManager::update( std::function<void( )> const& drawFunc )
{
	gl::enable( GL_POLYGON_OFFSET_FILL );
	glPolygonOffset( 2.0f, 2.0f );
	gl::ScopedFramebuffer framebuffer( mFbo );
	gl::ScopedViewport viewport( mFbo->getSize( ) );
	gl::ScopedGlslProg glsl( cinder::gl::getStockShader( cinder::gl::ShaderDef( ).texture( ) ) );
	gl::clear( );
	gl::enableDepth( );
	gl::enableAlphaBlending( );
	gl::setMatrices( mCamera );
	drawFunc( );
	gl::disable( GL_POLYGON_OFFSET_FILL );
}
void cShaderManager::draw( std::function<void( )> const& render )
{
	dot( vec3(), vec3() );

	gl::ScopedGlslProg scpGlsl( mGlsl );

	mGlsl->uniform( "uAmb", ColorA( 99 / 255.0F, 161 / 255.0F, 255 / 255.0F, 1.0F ) );

	mGlsl->uniform( "uPointLightNum", 0 );
	mGlsl->uniform( "uLineLightNum", 0 );

	gl::ScopedTextureBind texScope( mShadowTex, (uint8_t)1 );
	vec3 lightPos = vec3( CAMERA->getCamera( ).getViewMatrix( ) * vec4( mCamera.getEyePoint( ), 1.0f ) );
	mat4 shadowView = mCamera.getProjectionMatrix( ) * mCamera.getViewMatrix( );
	mGlsl->uniform( "uShadowMap", 1 );
	mGlsl->uniform( "uLightPos", lightPos );
	mGlsl->uniform( "uShadowView", shadowView );

	render( );
}
}
