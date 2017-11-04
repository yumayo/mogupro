#include <Game/cShaderManager.h>
#include <cinder/gl/gl.h>
#include <Game/cGemManager.h>
#include <CameraManager/cCameraManager.h>
using namespace ci;
namespace Game
{
void cShaderManager::setup( )
{
	mGlsl = cinder::gl::GlslProg::create( cinder::app::loadAsset( "Shader/world.vert" ),
										  cinder::app::loadAsset( "Shader/world.frag" ) );
}
cinder::gl::GlslProgRef cShaderManager::getScopedGlsl( )
{
	ci::gl::ScopedGlslProg scpGlsl( mGlsl );
	mGlsl->uniform( "uAmb", ColorA( 99 / 255.0F, 161 / 255.0F, 255 / 255.0F, 1.0F ) );
	std::vector<vec4> lightPositions;
	std::vector<vec4> lightColors;
	int lightNum = glm::min( (int)GemManager->getGems( ).size( ), 100 );
	mGlsl->uniform( "uLightNum", lightNum );
	for ( int i = 0; i < lightNum; ++i )
	{
		lightPositions.emplace_back( CAMERA->getCamera( ).getViewMatrix( ) *
									 vec4( vec3( GemManager->getGems( )[i]->getPos( ) ), 1 ) );
		lightColors.emplace_back( vec4( vec3( GemManager->getGems( )[i]->getColor( ) ), 1 ) );
	}
	mGlsl->uniform( "uModelViewLightPositions", lightPositions.data( ), lightNum );
	mGlsl->uniform( "uModelViewLightColors", lightColors.data( ), lightNum );
	return mGlsl;
}
}
